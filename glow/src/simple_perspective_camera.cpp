// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "simple_perspective_camera.h"
#include "geometry.h"

#include <iostream>


using namespace glow;


SimplePerspectiveCamera::SimplePerspectiveCamera(
    const Point3& pos, 
    const Vector3& dir, 
    const Vector3& up, 
    float dist) : Camera(pos, dir, up), distance(dist) {}


SimplePerspectiveCamera::~SimplePerspectiveCamera() {}


// For perspective cameras, the eye stays constant and the direction vectors move.
// 
//        ^
//       /
//      /
//     /
//  <) ----->
//     \
//      \
//       \
//        >
// 
//  <) : eye
//  -> : lookat
//
Ray SimplePerspectiveCamera::GetRay(
    const Vector3& xv, 
    const Vector3& yv, 
    const Vector3& w) const {

    Vector3 d = xv + yv + (w * distance);
    d.normalize();
    return Ray(position, d);
}



