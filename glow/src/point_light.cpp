// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "point_light.h"


using namespace glow;


Vector3 PointLight::IntensityImpl(const Point3& p, const Vector3&) const { 
    // Inverse Square Law
    auto delta = p - position;
    auto i = 1.0f / sqrt( 
        delta[0]*delta[0] + 
        delta[1]*delta[1] + 
        delta[2]*delta[2] );
    return intensity * i;
}
