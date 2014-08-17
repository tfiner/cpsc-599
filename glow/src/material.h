// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "color.h"
#include <memory>


namespace glow {


    struct HitRecord;
    class Light;
    typedef std::unique_ptr<Light> LightPtr;

    // This is the material that all other materials are derived from.
    // This class also uses the NVI idiom (like Object).
    class Material {
    public:
        virtual ~Material() {}
        Color Shade( const HitRecord& hr, const LightPtr& l ) const { return ShadeImpl(hr,l); }

    private:
        // Until overriden, the parent material returns a constant red color.
        virtual Color ShadeImpl(const HitRecord&, const LightPtr&) const { return Color(0.0f, 0.0f, 1.0f); }
    };


} // namespace glow
