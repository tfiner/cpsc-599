// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#pragma once


#include "geometry.h"


namespace glow {


    struct HitRecord {
        HitRecord() : t(-1.0) {}
        Vector3     view;           // The view vector (from the eye).
        float       t;              // How far along the ray when it intersected an object.
        Point3      surfHit;        // Where on the object was a hit?
        Vector3     surfNormal;     // The object surface normal.
        int         materialIndex;  // Which material?
    };


}