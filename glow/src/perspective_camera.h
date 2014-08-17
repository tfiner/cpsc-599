// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "camera.h"


namespace glow {


    class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera(const Point3& pos, const Vector3& dir, const Vector3& up_, float fov_);
        ~PerspectiveCamera();

    private:
//        float fov;

        virtual Ray GetRay(
            const Vector3& xv, 
            const Vector3& yv, 
            const Vector3& w) const override;
    };

    
} // namespace glow {
