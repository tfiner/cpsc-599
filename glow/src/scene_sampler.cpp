// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566

#include "scene_sampler.h"

#include "geometry.h"
#include "hit_record.h"
#include "scene.h"
#include "object.h"
#include "log_msg.h"


using namespace glow;


bool SceneSampler::Closest(const Scene& s, const Ray& ray, HitRecord& hr) const {
    return ClosestImpl(s, ray, hr);
}


bool RayCaster::ClosestImpl(const Scene& s, const Ray& ray, HitRecord& hrClosest) const {
    hrClosest.t = std::numeric_limits<float>::max();

    for( auto const & o : s.GetObjects() ) {
        HitRecord hr;
        if ( !o->Hit(hr, ray) )
            continue;

        if ( hr.t < hrClosest.t )
            hrClosest = hr;
    }
    return hrClosest.t != std::numeric_limits<float>::max();
}


bool RayMarcher::ClosestImpl(const Scene& s, const Ray& ray, HitRecord& hr) const {
    Point3 p(ray.o);

    bool insideScene = true;
    bool insideObject = false;

    auto deltaD = ray.d * delta;
    while ( (insideScene = far.Inside(p)) && !insideObject ) {
        for( auto const & o : s.GetObjects() ) {
            insideObject = o->Inside(hr, p);
            if ( insideObject ) {
                hr.t = Vector3(ray.o + p).norm();
                break;
            }
        }

        p += deltaD;
    }

    return insideObject;
}
