// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "light.h"
#include <trng/config.hpp>
#include <trng/yarn2.hpp>
#include <trng/uniform_dist.hpp>


namespace glow {


    struct Color;
    

    class AmbientOccluderLight : public Light {
    public:
        AmbientOccluderLight(const Color& c, const Vector3& i);

    private:
        mutable trng::yarn2 rng;
        mutable trng::uniform_dist<float> dist;

        virtual Vector3 DirectionImpl(const Point3& p, const Vector3& n) const override;
        virtual Vector3 ShadowImpl(const Point3&p, const Vector3& n) const override;

        Vector3 RandPerturb(const Vector3& vin) const;
    };


}
