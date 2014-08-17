// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//


#include "matte.h"
#include "hit_record.h"
#include "light.h"
#include "log_msg.h"

#include <cassert>


using namespace glow;


Color Matte::ShadeImpl(const HitRecord& hr, const LightPtr& l) const {
    // Lambertian reflectance:
    // (L dot N) C I
    //
    // L: Light vector from surface to light.
    // N: Surface Normal 
    // C: Color 
    // I: Intensity

    auto const L = -l->Direction(hr.surfHit, hr.surfNormal);
    auto LDotN = L.dot(hr.surfNormal);
    // LOG_MSG(2, "LDotN: " << LDotN); 

    LDotN = std::max(LDotN, 0.0f);

    Color light(l->GetColor());
    auto const I = l->Intensity(hr.surfHit, hr.surfNormal);

    light.r *= I[0] * LDotN;
    light.g *= I[1] * LDotN;
    light.b *= I[2] * LDotN;

    // LOG_MSG(2, "light: " 
    //     << light.r << " "
    //     << light.g << " "
    //     << light.b );

    Color reflected(matte);

    // LOG_MSG(2, "matte: " 
    //     << matte.r << " "
    //     << matte.g << " "
    //     << matte.b );

    reflected.r *= light.r;
    reflected.g *= light.g;
    reflected.b *= light.b;

    LOG_MSG(4, "reflected: " 
        << reflected.r << " "
        << reflected.g << " "
        << reflected.b );

    return reflected;
}
