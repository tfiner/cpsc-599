// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "light.h"


namespace glow {


    struct Color;
    

    class DirectionLight : public Light {
    public:
        DirectionLight(const Color& c, const Vector3& i, const Vector3& d) :  Light(c,i), direction(d) {}

    private:
        Vector3 direction;
        virtual Vector3 DirectionImpl(const Point3&, const Vector3&) const override { return direction; }
        virtual Vector3 ShadowImpl(const Point3&, const Vector3&) const override { return -direction; }
    };


}
