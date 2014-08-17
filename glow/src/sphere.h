// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//


#pragma once


#include "object.h"
#include "geometry.h"


namespace glow {


    struct HitRecord;
    struct Ray;


    class Sphere : public Object {
    public:
        Sphere(const Point3& center_, float radius_, int matIdx) : 
            Object(matIdx), center(center_), radius(radius_) {}

    private:
        bool HitImpl( HitRecord& hr, const Ray& r ) const override;
        bool InsideImpl( const Point3& p ) const override;

        Vector3 GetNormal(const Point3& pt) const override;

        Point3 center;
        float radius;
    };


} // namespace glow
