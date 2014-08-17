// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "material.h"
#include "matte.h"


namespace glow {


    struct HitRecord;
    
    class Phong : public Material {
    public:
        Phong(const Color& diffuse_, const Color& specular_, float hardness_) : 
            diffuse(diffuse_), specular(specular_), hardness(hardness_) {}

        virtual ~Phong() {}

    private:
        Matte diffuse;
        Color specular;
        float hardness;
        Color ShadeImpl(const HitRecord&, const LightPtr&) const override;
    };


} // namespace glow
