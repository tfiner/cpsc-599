// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#pragma once


#include "color.h"
#include "material.h"


namespace glow {


    struct HitRecord;

    // A Lambertian shader that calculates using the surface normal vs. 
    // the incoming light vector.
    class Matte : public Material {
    public:
        Matte(const Color& c) : matte(c) {}

    private:
        Color matte;
        Color ShadeImpl(const HitRecord& hr, const LightPtr& l) const override;
    };


} // namespace glow
