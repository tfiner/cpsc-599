// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "color.h"
#include "geometry.h"


namespace glow {


    class Light {
    public:
        Light(const Color& c, const Vector3& i) : color(c), intensity(i) {}
        virtual ~Light() {}

        Color GetColor() const { return color; }

        // Returns the intensity at a given point in world space.
        Vector3 Intensity(const Point3& p, const Vector3& n) const { return IntensityImpl(p,n); }

        // Returns the direction at the given point and given normal in world space.
        Vector3 Direction(const Point3& p, const Vector3& n) const { return DirectionImpl(p,n); }

        // Returns the shadow vector, given point p and the given normal to the light.
        Vector3 Shadow(const Point3& p, const Vector3& n) const { return ShadowImpl(p,n); }

    protected:
        Color color;
        Vector3 intensity;

        virtual Vector3 IntensityImpl(const Point3&, const Vector3&) const { return intensity; }
        virtual Vector3 DirectionImpl(const Point3& p, const Vector3& n) const = 0;
        virtual Vector3 ShadowImpl(const Point3& p, const Vector3& n) const = 0;
    };


}
