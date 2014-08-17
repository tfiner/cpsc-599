// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//


#pragma once


// These macros define the lambda for use in the loop to render and
// the loop that does the rendering.  I moved all these in here because it 
// really bugged me seeing the ifdefs in the code.


#define RAYTRACER_COMMON \
    auto samples = sampler->GenerateN(s.GetNumSamples());                               \
    auto results = RenderPixel(s, samples, fp, x, y);                                   \
    colorFilm[y * width + x] = results.color;                                           \
    if ( results.closest != std::numeric_limits<float>::max() )                         \
        depthFilm[y * width + x] = results.closest;



// The first one uses Intel's TBB to use all cores.
#if defined(USE_TBB)

#   define RAYTRACER_LAMBDA                                                                             \
        auto raytrace = [&](const tbb::blocked_range2d<int>& range){                                    \
            auto sampler = s.CloneSampler(); /* These need to be allocated on a per thread basis. */    \
            for( auto y = range.rows().begin(); y != range.rows().end(); ++y ) {                        \
                for( auto x = range.cols().begin(); x != range.cols().end(); ++x ) {                    \
                    RAYTRACER_COMMON                                                                    \
                }                                                                                       \
            }                                                                                           \
        };

#   define RAYTRACER_LOOP                                                                               \
        tbb::parallel_for(                                                                              \
            tbb::blocked_range2d<int>(0, height, 1, 0, width, 1),                                       \
            raytrace                                                                                    \
        );

// The single threaded version.
#else 

#   define RAYTRACER_LAMBDA                                                                             \
        auto sampler = s.CloneSampler();                                                                \
        auto raytrace = [&](int x, int y) {                                                             \
            RAYTRACER_COMMON                                                                            \
        };

#   define RAYTRACER_LOOP                                                                               \
    for ( auto y = 0; y < height; ++y ) {                                                               \
        for ( auto x = 0; x < width; ++x ) {                                                            \
            raytrace(x,y);                                                                              \
        }                                                                                               \
    }

#endif