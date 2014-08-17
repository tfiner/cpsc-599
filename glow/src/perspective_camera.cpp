// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "perspective_camera.h"
#include "geometry.h"

#include <iostream>


using namespace glow;


PerspectiveCamera::PerspectiveCamera(
    const Point3& pos, 
    const Vector3& dir, 
    const Vector3& up, 
    float /*fov_*/) : Camera(pos, dir, up)/*, fov(fov_)*/ {}


PerspectiveCamera::~PerspectiveCamera() {}


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
Ray PerspectiveCamera::GetRay(
    const Vector3& xv, 
    const Vector3& yv, 
    const Vector3& w) const {

    Vector3 d = xv + yv + w /* TODO: viewplane distance using fov */;
    d.normalize();
    return Ray(position, d);
}



