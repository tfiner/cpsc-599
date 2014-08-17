// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "color.h"
#include "material.h"
#include <memory>


namespace glow {


    struct HitRecord;
    class Light;
    typedef std::unique_ptr<Light> LightPtr;
    
    // A debug class that ignores light and just returns a constant color.
    class ConstantShader : public Material {
    public:
        ConstantShader(const Color& c) : constant(c) {}

    private:
        Color constant;
        Color ShadeImpl(const HitRecord&, const LightPtr&) const override { return constant; }
    };


} // namespace glow
