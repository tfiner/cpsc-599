// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "camera.h"


namespace glow {


    class SimplePerspectiveCamera : public Camera {
    public:
        SimplePerspectiveCamera(const Point3& pos, const Vector3& dir, const Vector3& up_, float dist);
        ~SimplePerspectiveCamera();

        float GetDistance() const { return distance; }

    private:
        float distance;

        virtual Ray GetRay(
            const Vector3& xv, 
            const Vector3& yv, 
            const Vector3& w) const override;
    };

    
} // namespace glow {
