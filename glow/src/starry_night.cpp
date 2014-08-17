// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "starry_night.h"
#include "scene.h"
#include "geometry.h"

#include "log_msg.h"


using namespace glow;


StarryNight::StarryNight(float density_, float minLevel, float maxLevel) : 
    density(density_), exist(0.0,1.0), grayLevel(minLevel,maxLevel) {

}


void StarryNight::GetColorImpl(const Scene& s, const Ray& r, Color& c, int /*x*/, int /*y*/) const {
    // Dot with Up is 1.0 at horizon, and 0 at zenith.
    if (s.IsFoggy()){
        const Vector3 UP(0,0,1.0f);
        auto blend = (1 - r.d.dot(UP)) * s.GetFogDensity();
        LOG_MSG(4, "ray:\n" << r.d << " blend: " << blend);
        c = s.GetFogColor();
        c.r *= blend;
        c.g *= blend;
        c.b *= blend;    
    }

    if (exist(rng) >= (1 - density)) {
        auto level = grayLevel(rng);
        c = Color(level, level, level);
    }            
}
