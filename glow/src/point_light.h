// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "light.h"


namespace glow {


    struct Color;
    

    class PointLight : public Light {
    public:
        PointLight(const Color& c, const Vector3& i, const Vector3& pos) : Light(c,i), position(pos) {}

    private:
        Point3 position;
        virtual Vector3 IntensityImpl(const Point3& p, const Vector3& n) const override;
        virtual Vector3 DirectionImpl(const Point3& p, const Vector3&) const override { 
            return p - position; 
        }
        
        virtual Vector3 ShadowImpl(const Point3& p, const Vector3&) const override { 
            return Vector3(position - p).normalized(); 
        }
    };


}
