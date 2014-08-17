// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "ambient_occluder_light.h"


using namespace glow;


namespace {


    const Vector3 MOSTLY_UP(0.0072, 1.0, 0.0034);


}


AmbientOccluderLight::AmbientOccluderLight(const Color& c, const Vector3& i) : 
    Light(c,i),  dist(-1.0,1.0) {}


Vector3 AmbientOccluderLight::DirectionImpl(const Point3&, const Vector3& n) const {
    return RandPerturb(n);
}


Vector3 AmbientOccluderLight::ShadowImpl(const Point3&, const Vector3& n) const { 
    return RandPerturb(n);
}


Vector3 AmbientOccluderLight::RandPerturb(const Vector3& vin) const {
    auto w = vin.normalized();
    // Jitter the up vector in case the normal is straight up.
    auto v = w.cross(MOSTLY_UP).normalized();
    auto u = v.cross(w);

    Point3 sp;
    sp[0] = dist(rng);
    sp[1] = dist(rng);
    sp[2] = dist(rng);

    auto n = sp[0] * u + sp[1] * v + sp[2] * w;
    return n.normalized();
}
