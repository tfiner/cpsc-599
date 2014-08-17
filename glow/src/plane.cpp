// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//


#include "plane.h"
#include "hit_record.h"

#include <cassert>
#include <cmath>


using namespace glow;


namespace {


    const auto EPSILON = 0.00001;


} // namespace {


bool Plane::HitImpl( HitRecord& hr, const Ray& r ) const {
    assert( (1.0f - fabs(r.d.norm())) < EPSILON );

    float denom = normal.dot(r.d);
    if (denom > EPSILON) {
        Vector3 planeToRay = point - r.o;
        hr.t = planeToRay.dot(normal) / denom; 
        return hr.t >= 0;
    }
    return false;
}


Vector3 Plane::GetNormal( const Point3& ) const {
    return normal;
}


bool Plane::InsideImpl( const Point3& p ) const {
    Vector3 w = point - p;
    return -normal.dot(w) >= 0;
}

