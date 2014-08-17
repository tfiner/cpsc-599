// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "object.h"
#include "hit_record.h"
#include "log_msg.h"


using namespace glow;


bool Object::Hit( HitRecord& hr, const Ray& r ) const { 
    auto didHit = HitImpl(hr, r);

    if ( didHit ) {
        hr.surfHit          = r.o + (r.d * hr.t);
        hr.materialIndex    = materialIndex;
        hr.surfNormal       = GetNormal( hr.surfHit );
    } 

    return didHit;
}


bool Object::Inside( HitRecord& hr, const Point3& p ) const { 
    auto inside = InsideImpl(p);

    if ( inside ) {
        hr.surfHit          = p;
        hr.materialIndex    = materialIndex;
        hr.surfNormal       = GetNormal( p );
    } 

    return inside;
}

bool Object::Inside( const Point3& p ) const { 
    return InsideImpl(p);
}
