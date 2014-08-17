// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "material.h"
#include "hit_record.h"

#include <memory>


namespace glow {


    class MaterialGradient : public Material {
    public:
        virtual ~MaterialGradient() {}

    private:
        virtual Color ShadeImpl(const HitRecord& hr, const LightPtr&) const override { 
            float gray = std::min(1.0f, std::max(0.0f, -hr.surfHit[2]));
            return Color(gray, gray, gray); 
        }
    };


} // namespace glow
