// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//
// This file abstracts away the ray tracing algorithms into an abstraction of
// scene sampling using a ray as a source.  This is an example of the Strategy Pattern.
// 
// This was designed so that alternative ray casting strategies like ray 
// marching can be dropped in.  

// I'm still thinking about a way to pick an optimal ray casting algorithms dynamically.
// Some objects can be ray casted (like spheres, etc.) while some objects are better 
// ray marched (clouds, mandelbulbs, implicit surfaces).

#pragma once


#include "plane.h"


namespace glow {


    class Scene;
    struct Ray;
    struct HitRecord;


    class SceneSampler {
    public:
        // Returns true and fills in the hit record closest if something was hit. 
        bool Closest(const Scene& s, const Ray& ray, HitRecord& hr) const;

    private:
        virtual bool ClosestImpl(const Scene& s, const Ray& ray, HitRecord& hr) const = 0;
    };


    // This is the first scene sampler I wrote.  It does basic ray casting (tracing).
    class RayCaster : public SceneSampler {
    private:
        virtual bool ClosestImpl(const Scene& s, const Ray& ray, HitRecord& hr) const override;
    };

    // This scene sampler accumulates samples along the ray, see:
    // http://en.wikipedia.org/wiki/Volume_ray_casting
    class RayMarcher : public SceneSampler {
    public:
        // The ray marcher needs a far plane where marching stops. 
        RayMarcher( const Plane& far_, float delta_ ) : far(far_), delta(delta_) {}

    private:
        Plane far;
        float delta;
        virtual bool ClosestImpl(const Scene& s, const Ray& ray, HitRecord& hr) const override;
    };


} // namespace glow
