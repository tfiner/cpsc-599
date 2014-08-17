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


    class Plane : public Object {
    public:
        Plane(const Point3& pt_, const Vector3& normal_, int matIdx) : 
            Object(matIdx), point(pt_), normal(normal_) {}

    private:
        bool HitImpl( HitRecord& hr, const Ray& r ) const override;
        bool InsideImpl( const Point3& p ) const override;

        Vector3 GetNormal(const Point3& pt) const override;

        Point3 point;
        Vector3 normal;
    };


} // namespace glow
