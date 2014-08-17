// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "orthogonal_camera.h"
#include "scene.h"
#include "view_plane.h"
#include "geometry.h"
#include "recorder.h"
#include "object.h"
#include "hit_record.h"
#include "material.h"
#include "log_msg.h"

#include <iostream>


using namespace glow;


OrthogonalCamera::OrthogonalCamera(const Point3& pos, const Vector3& dir, const Vector3& up) :
    Camera(pos, dir, up) {}


OrthogonalCamera::~OrthogonalCamera() {}

// For orthogonal cameras the eye moves and the direction remains constant.
// 
//  <) ----->
//  <) ----->
//  <) ----->
// 
//  <) : eye
//  -> : lookat
//
Ray OrthogonalCamera::GetRay(
    const Vector3& xv, 
    const Vector3& yv, 
    const Vector3& w) const {

    auto eye = position + xv + yv;
    return Ray(eye, w);
}
