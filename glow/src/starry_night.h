// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//
// Named after the Van Gogh painting The Starry Night, this background
// sampler renders stars.
//

#pragma once 

#include "background_sampler.h"
#include "color.h"

#include <trng/config.hpp>
#include <trng/yarn2.hpp>
#include <trng/uniform_dist.hpp>


namespace glow {


    class StarryNight : public BackgroundSampler {
    public:
        StarryNight(float density, float minLevel, float maxLevel);
        StarryNight() = delete;

    private:
        float density;
        Color background;
        mutable trng::yarn2 rng;
        mutable trng::uniform_dist<float> exist;
        mutable trng::uniform_dist<float> grayLevel;

        void GetColorImpl(const Scene& s, const Ray& r, Color& c, int x, int y) const override;
    };

}