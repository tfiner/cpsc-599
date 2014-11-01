// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#include "camera.h"
#include "scene.h"
#include "view_plane.h"
#include "geometry.h"
#include "recorder.h"
#include "object.h"
#include "hit_record.h"
#include "material.h"
#include "light.h"
#include "sampler.h"
#include "scene_sampler.h"
#include "log_msg.h"

#include <tbb/tbb.h>

#include <iostream>
#include <algorithm>


using namespace glow;


namespace {


    // Returns true if there is a line of sight from the 
    // point p to the light l, given the objects objs.
    bool IsLineOfSight(const HitRecord& hrIn, const LightPtr& l, const ObjectSeq& objs) {
        Ray ray(hrIn.surfHit, l->Shadow(hrIn.surfHit, hrIn.surfNormal));
        bool clearPath = true;

        // The first object this ray hits means that there is not a clear path.
        HitRecord hr;
        for ( auto const & o : objs ) {
            if ( o->Hit(hr, ray) ) {
                clearPath = false;
                break;
            }
        }

        return clearPath;
    }


    // Calculates the light contribution and adds it to pixel. 
    void Expose(const Scene& s, const MaterialPtr& mat, 
        const HitRecord& hr, Color& pixel) {

        for ( auto const & l : s.GetLights() ) {
            // If in shadow.
            if ( !IsLineOfSight(hr, l, s.GetObjects()) )
                continue;

            Color c = mat->Shade(hr, l);
            pixel.r += c.r;
            pixel.g += c.g;
            pixel.b += c.b; 
        }
    }


    void AverageColor(RenderResults& results) {
        results.color.r /= results.hits;
        results.color.g /= results.hits;
        results.color.b /= results.hits;        
    }


    void RenderAmbient(const Scene& s, Color& c) {
        auto const ambient = s.GetAmbient();
        c.r += ambient.r;
        c.g += ambient.g;
        c.b += ambient.b;        
    }


    void RenderFog(const Scene& s, RenderResults& results){
        if ( !s.IsFoggy() ) 
            return;

        auto f = 0.0;
        if ( results.closest == std::numeric_limits<float>::max() )
            f = 1.0;
        else {
            auto delta = std::max(0.0f, s.GetFogMaximum() - results.closest);
            f = 1.0 - (delta / s.GetFogMaximum());
        }

        f *= s.GetFogDensity();
        f = std::max(f, 0.0);
        f = std::min(f, 1.0);
        LOG_MSG(4, "f: " << f << " closest: " << results.closest);
        
        auto fogColor = s.GetFogColor();
        results.color.r = std::min(1.0, results.color.r + (fogColor.r * f));
        results.color.g = std::min(1.0, results.color.g + (fogColor.g * f));
        results.color.b = std::min(1.0, results.color.b + (fogColor.b * f));
    }


} // namespace {


RenderResults::RenderResults(const Scene&) : 
    closest(std::numeric_limits<float>::max()), 
    hits(0) {}


Camera::Camera(const Point3& pos, const Vector3& dir, const Vector3& up_) :
    position(pos), direction(dir), up(up_), roll(0.0) {}


Camera::~Camera() {}


void Camera::Render( Scene& s ) {
    auto const & vp         = s.GetViewPlane();

    auto xBegin = 0u;
    auto xEnd   = 0u;
    std::tie(xBegin, xEnd)  = vp->GetHSpanPixels();

    auto yBegin = 0u;
    auto yEnd   = 0u;
    std::tie(yBegin, yEnd)  = vp->GetVSpanPixels();

    auto const width        = xEnd - xBegin; 
    auto const height       = yEnd - yBegin;

    LOG_MSG(1, "Rendering " 
        << "x0: " << xBegin << " "
        << "x1: " << xEnd << " "
        << "y0: " << yBegin << " "
        << "y1: " << yEnd << " "
        << "width: " << width << " "
        << "height: " << height);

    if (width == 0 || height == 0) {
        LOG_MSG(0, "Returning without rendering, " 
            << "width: " << width << " "
            << "height: " << height);
        return;
    } 
    auto colorFilm = ColorFilm( width * height, Color(0,0,0) );
    LOG_MSG(0, 
        "color file size: " << colorFilm.size()
    );


    DepthFilm depthFilm( width * height, -1.0f );

    FrameParams fp;
    fp.pixelSize = vp->GetPixelSize();

    fp.xBegin   = xBegin;
    fp.yBegin   = yBegin;
    fp.centerX  = (xBegin + xEnd - 1) * 0.5;
    fp.centerY  = (yBegin + yEnd - 1) * 0.5;

    // Create our orthonormal basis, using Suttern, pg. 159.
    fp.w = direction.normalized();
    fp.u = up.cross(fp.w).normalized();
    fp.v = fp.w.cross(fp.u);

    Roll(fp.u, fp.v, fp.w);

    auto raytrace = [&colorFilm, &s, this, &fp, &depthFilm,
                    yBegin, xBegin, width](const tbb::blocked_range2d<unsigned int>& range){
        // The sampler must be allocated on a per thread basis.
        auto sampler = s.CloneSampler(); 
        auto const numSamples = s.GetNumSamples();

        auto const y0 = range.rows().begin();
        auto const y1 = range.rows().end();
        auto const x0 = range.cols().begin();
        auto const x1 = range.cols().end();
        // LOG_MSG(0, 
        //     "x0: " << x0 << " x1: " << x1 << " " 
        //     "y0: " << y0 << " y1: " << y1 
        // );

        for( auto y = y0; y < y1; ++y ) {
            for( auto x = x0; x < x1; ++x ) {
                auto const samples = sampler->GenerateN(numSamples);
                auto const results = RenderPixel(s, samples, fp, x, y);
                auto const offset  = (y - yBegin) * width + (x - xBegin);
                assert(offset < colorFilm.size());
                colorFilm[offset] = results.color;
                if ( results.closest != std::numeric_limits<float>::max() )
                    depthFilm[offset] = results.closest;
            }
        }
    };

    tbb::parallel_for(
        tbb::blocked_range2d<unsigned int>(yBegin, yEnd, 8, xBegin, xEnd, 8),
        raytrace
    );    

// Keep around for testing.
#if 0
    auto sampler = s.CloneSampler(); 
    auto samples = sampler->GenerateN(s.GetNumSamples());
    for ( auto y = yBegin; y < yEnd; ++y ) {
        for ( auto x = xBegin; x < xEnd; ++x ) {
            auto results = RenderPixel(s, samples, fp, x, y);
            auto offset  = (y - yBegin) * width + (x - xBegin);
            assert(offset < colorFilm.size());
            colorFilm[offset] = results.color;
            // if ( results.closest != std::numeric_limits<float>::max() )
            //     depthFilm[offset] = results.closest;            
        }
    }
#endif

    s.GetColorRecorder()->RecordColor( colorFilm );

    auto & depthRecorder = s.GetDepthRecorder();
    if (depthRecorder)
        depthRecorder->RecordDepth( depthFilm );                
}


void Camera::Roll(Vector3& u, Vector3& v, Vector3& w) const {
    if (0.0f == roll)
        return;

    Eigen::Transform<float, 3, Eigen::Affine> t(Eigen::AngleAxis<float>(roll,w));
    u = t * u;
    v = t * v;
}


RenderResults Camera::RenderPixel(const Scene& s, 
                         const Samples& samples, 
                         const FrameParams& fp, 
                         int x, int y) const {

    auto const & sceneSampler = s.GetSceneSampler();

    RenderResults results(s);
    Ray lastRay; // used for GetBackground;
    for( auto const & sample : samples) {
        auto const xv = (fp.pixelSize * (fp.xBegin + x - fp.centerX + sample[0])) * fp.u;
        auto const yv = (fp.pixelSize * (fp.yBegin + y - fp.centerY + sample[1])) * fp.v;
        auto ray = GetRay(xv, yv, fp.w);
        LOG_MSG( 4, "\n" << ray );

        lastRay = ray;

        HitRecord hr;
        if ( !sceneSampler->Closest(s, ray, hr) )
            continue;

        results.closest = std::min(results.closest, hr.t);
        results.hits++;
        hr.view = ray.d;

        assert(hr.materialIndex >= 0);
        assert(hr.materialIndex < static_cast<int>(s.GetMaterials().size()));
        auto & mat = s.GetMaterials().at(hr.materialIndex);
        Expose(s, mat, hr, results.color);
    }

    if ( results.hits ) {
        AverageColor(results);
        RenderAmbient(s, results.color);

    } else {
        results.color = s.GetBackground(lastRay, x, y);
    }

    RenderFog(s, results);
    return results;
}

