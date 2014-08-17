// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//


#include "phong.h"
#include "light.h"
#include "color.h"
#include "hit_record.h"
#include "log_msg.h"


using namespace glow;


Color Phong::ShadeImpl(const HitRecord& hr, const LightPtr& l) const {
    // Calculate diffuse
    auto const diff = diffuse.Shade(hr, l);

    // Light color, modulated by distance (if the light is affected by distance)
    Color lightC( l->GetColor() );
    auto const lightI = l->Intensity( hr.surfHit, hr.surfNormal );
    lightC.r *= lightI[0];
    lightC.g *= lightI[1];
    lightC.b *= lightI[2];

    // Blinn Phong, calculate the half angle.
    auto const L = l->Direction( hr.surfHit, hr.surfNormal );
    auto const H = (L + hr.view).normalized();
 
    // Specular light intensity
    auto const NdotH = hr.surfNormal.dot( H );
    auto intensity = pow( NdotH, hardness );

    lightC.r *= intensity;
    lightC.g *= intensity;
    lightC.b *= intensity;

    Color reflected(specular);    
    reflected.r *= lightC.r;
    reflected.g *= lightC.g;
    reflected.b *= lightC.b;

    // Add in the diffuse component.
    reflected.r += diff.r;
    reflected.g += diff.g;
    reflected.b += diff.b;

    return reflected;
}
