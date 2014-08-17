// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//


#include "sphere.h"
#include "hit_record.h"

#include <math.h>

#include <cassert>
#include <limits>
#include <iostream>


using namespace glow;


namespace {


    const auto EPSILON = 0.0001;


} // namespace {


bool Sphere::HitImpl( HitRecord& hr, const Ray& r ) const {
    // This code was taken from Suffern's original code for Ray/Sphere
    // intersection.  I wrote some python code to do it using vectors
    // and as far as I can tell, works in python, but...  I couldn't
    // get it to work correctly.

    auto const sphereToRay  = r.o - center;
    auto const a            = r.d.dot( r.d );
    auto const b            = Vector3( 2.0f * sphereToRay).dot( r.d );
    auto const c            = sphereToRay.dot( sphereToRay ) - radius * radius;
    auto const disc         = b * b - 4.0 * a * c;

    if (disc < 0.0)
        return false;

    auto const e = sqrt(disc);
    auto const denom = 2.0 * a;
    bool hit = false;

    hr.t = (-b - e) / denom;    // smaller root
    if (hr.t > EPSILON) {
        hit = true;
    }

    if ( !hit ) {
        hr.t = (-b + e) / denom;    // larger root
        if (hr.t > EPSILON) {
            hit = true;
        }
    }

    return hit;
}


Vector3 Sphere::GetNormal( const Point3& pt ) const {
    return Vector3(pt - center).normalized();
}


bool Sphere::InsideImpl( const Point3& p ) const {
    auto dist = sqrt((p - center).squaredNorm());
    return dist < radius;
}

