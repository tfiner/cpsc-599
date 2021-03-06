// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#include "recorder_png.h"
#include "log_msg.h"

#include <cassert>
#include <algorithm>


using namespace std;
using namespace glow;


RecorderPng::RecorderPng(const char* out, int w, int h, int depth, const char* comment) : 
    out_(out), w_(w), h_(h), depth_(depth), comment_(comment), gamma_(1.0f) {
    Init();
}


RecorderPng::~RecorderPng() { 
    Finish(); 
}


void RecorderPng::RecordColorImpl(const ColorFilm& film) {
    for (int y = 0; y < h_; ++y) {
        for (int x = 0; x < w_; ++x) {
            auto c = film[y * w_ + x]; 
            SetPixel(x, y, c.r, c.g, c.b);            
        }
    }
}

void RecorderPng::RecordDepthImpl(const DepthFilm& film) {
    if (film.empty())
        return;

    // Normalize the depth values to the range [0.0, 1.0] (inclusive),
    // ignoring < 0.
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    for ( auto f : film ) {
        if ( f < 0.0f )
            continue;
        min = std::min(min, f);
        max = std::max(max, f);
    }

    // If min or max are unchanged, then nothing was hit.
    auto rangeFactor = 1.0f;
    if( min == std::numeric_limits<float>::max() ||
        max == std::numeric_limits<float>::min() ) {
        LOG_MSG(1, "Nothing was hit." );

    } else {
        auto const range = max - min;
        rangeFactor = 1.0f / range;

        LOG_MSG(2, 
               "depth: " 
            << "min: " << min << ", "
            << "max: " << max << ", "
            << "range: " << range << ", "
            << "rangeFactor: " << rangeFactor
        );
    }

    for (int y = 0; y < h_; ++y) {
        for (int x = 0; x < w_; ++x) {
            auto f = film[y * w_ + x];

            if (f > 0.0f) {
                // Normalize to [0.0, 1.0]
                f = (f - min) * rangeFactor;
                LOG_MSG(3, "f: " << f);

            } else {
                f = 1.0f;
            }

            // Invert so that closer is brighter, darker is farther.
            f = 1.0f - f;

            // SetPixel will interpret these as RGB and make a gray pixel.
            SetPixel(x, y, f, f, f);
        }
    }
}


// void SetPixel8(std::vector<png_byte>& array, size_t idx, float red, float green, float blue) {
//     array[idx+0] = GammaEncode(red);
//     array[idx+1] = GammaEncode(green);
//     array[idx+2] = GammaEncode(blue);
// }


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void SetPixel16(std::vector<png_byte>& array, size_t idx, float gray) {
    assert(gray >= 0.0);
    assert(gray <= 1.0);

    png_uint_16 g = static_cast<png_uint_16>( 0x0fff );

    if ( g ){
        LOG_MSG( 3, "g: " << g );
    }

    auto p = reinterpret_cast<png_uint_16*>(&array[idx]);    
    *p = g;
}
#pragma clang diagnostic pop


// This implementation assumes that x is increasing and contiguous.
void RecorderPng::SetPixel(int x, int y, float red, float green, float blue) {
    // Bytes are tightly packed RGB RGB RGB
    const size_t idx = (y * w_ * pixSize_) + (x * pixSize_);

    if ( depth_ == 8 ) {
        // SetPixel8( array_bytes_, idx, red, green, blue );
        array_bytes_[idx+0] = GammaEncode(red);
        array_bytes_[idx+1] = GammaEncode(green);
        array_bytes_[idx+2] = GammaEncode(blue);

    } else if ( depth_ == 16 )
        SetPixel16( array_bytes_, idx, red );
}   


void RecorderPng::Finish() {
    Flush();

    png_write_end(png_ptr_, png_info_ptr_);
    png_destroy_write_struct(&png_ptr_, &png_info_ptr_);

    if (fp_)
        fclose(fp_);

    fp_ = nullptr;
}


void RecorderPng::Init() {
    pixSize_ = 3;
    if (depth_ == 16) 
        pixSize_ = 2;

    fp_ = fopen(out_.c_str(), "wb");
    if ( !fp_ )
        throw new runtime_error( "Couldn't open file " + out_ );

    png_ptr_ = png_create_write_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );

    if (!png_ptr_)
        throw new runtime_error( "Couldn't create png struct." );

    png_info_ptr_ = png_create_info_struct(png_ptr_);
    if (!png_info_ptr_)
        throw new runtime_error( "Couldn't create png info struct. " );

    png_init_io(png_ptr_, fp_);
    png_set_compression_level(png_ptr_, Z_DEFAULT_COMPRESSION);

    auto colorType = PNG_COLOR_TYPE_RGB;
    if ( depth_ == 16 )
        colorType = PNG_COLOR_TYPE_GRAY;

    png_set_IHDR(
                 png_ptr_, 
                 png_info_ptr_, 
                 w_, h_,
                 depth_, 
                 colorType, 
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, 
                 PNG_FILTER_TYPE_DEFAULT
                 );

    png_text  text[1];
    text[0].key         = (png_charp)("599-tf-scene");
    text[0].text        = (png_charp)(comment_.c_str());
    text[0].compression = PNG_TEXT_COMPRESSION_NONE;
    png_set_text(png_ptr_, png_info_ptr_, text, 1);

    if ( depth_ == 16 ){
        png_color_8_struct bits = { 0, 0, 0, 16, 0 };
        png_set_sBIT(png_ptr_, png_info_ptr_, &bits);
    }

    png_write_info(png_ptr_, png_info_ptr_);
    png_set_packing(png_ptr_);

    array_bytes_ = ArrayBytes(w_ * h_ * pixSize_);
}


void RecorderPng::Flush(){
    // Flip the image so that it is right side up.
    for ( int h = h_-1; h >= 0; --h ) {
        const size_t row = h * (w_ * pixSize_);
        png_write_row(png_ptr_, &array_bytes_[row] );
    }
}

png_byte RecorderPng::GammaEncode(float value) const {
    auto corrected  = std::pow(value, gamma_) * 255.0f;
    auto clamped    = std::max(0.0f, std::min(corrected, 255.0f));
    return static_cast<png_byte>(clamped);
}


