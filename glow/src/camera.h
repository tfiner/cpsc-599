// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "geometry.h"
#include "color.h"


#include <vector>


namespace glow {


    class Scene;
    using ColorFilm = std::vector<Color>;
    using Samples = std::vector<Vector3>;

    // These are the constants during a render of an entire frame.
    struct FrameParams {
        float pixelSize;
        float centerX;
        float centerY;
        Vector3 u, v, w;
    };

    // This returns the results of rendering a pixel.
    struct RenderResults {
        RenderResults(const Scene& s);
        Color color;
        float closest;
        float hits;
    };


    class Camera {
    public:
        Camera(const Point3& pos, const Vector3& dir, const Vector3& up_);
        virtual ~Camera();

        void Render( Scene& s );

        const Point3& Position() const { return position; }
        const Vector3& Direction() const { return direction; }

    protected:
        Point3  position;
        Vector3 direction;
        Vector3 up;

        // Roll angle in radians around direction.
        float   roll;

    private:
        // The difference between Orthogonal and Perspective cameras
        // is boiled down to this call.

        // xv and yv are normalized viewplane vectors that have already
        // been scaled by the orthonormal basis u and v vectors.
        // w is part of the orthonomal basis.

        virtual Ray GetRay(const Vector3& xv, const Vector3& yv, const Vector3& w) const = 0;

        // Roll the camera about the direction axis.
        void Roll(Vector3&, Vector3&, Vector3&) const;

        // Renders a single pixel.
        RenderResults RenderPixel(const Scene& s, 
                                 const Samples& samples, 
                                 const FrameParams& fp, 
                                 int x, int y);

    };

    
} // namespace glow {
