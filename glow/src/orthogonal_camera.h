// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "camera.h"


namespace glow {


    class OrthogonalCamera : public Camera {
    public:
        OrthogonalCamera(const Point3& pos, const Vector3& dir, const Vector3& up_);
        ~OrthogonalCamera();

    private:
        Ray GetRay(
            const Vector3& xv, 
            const Vector3& yv, 
            const Vector3& w) const override;
    };

    
} // namespace glow {
