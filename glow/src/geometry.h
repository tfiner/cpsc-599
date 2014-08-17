// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"

#include <Eigen/Dense>

#pragma clang diagnostic pop

#pragma once

#include <math.h>
#include <iosfwd>


namespace glow {


    using Matrix4 = Eigen::Matrix<float, 4, 4>;
    using Vector3 = Eigen::Vector3f;
    using Point3  = Eigen::Vector3f;


    struct Ray {
        Ray() : o(0.0f, 0.0f, 0.0f), d(0.0f,0.0f,1.0f) {}
        Ray(const Point3& o_, const Vector3& d_) : o(o_), d(d_) {}
        Point3     o;
        Vector3    d;  // direction
    };


#if 0
    template <typename DerivedA,typename DerivedB>
    inline typename DerivedA::Scalar distance(  const Eigen::MatrixBase<DerivedA>& p1, 
                                                const Eigen::MatrixBase<DerivedB>& p2) {
      return sqrt((p1-p2).squaredNorm());
    }

    template <typename DerivedA,typename DerivedB>
    inline DerivedA project(    const Eigen::MatrixBase<DerivedA>& a, 
                                const Eigen::MatrixBase<DerivedB>& b ) {

        auto aDotb      = a.dot(b);
        auto bDotb      = b.dot(b);
        auto abOverbb   = aDotb / bDotb;
        return b * abOverbb;
    }
#endif
} // glow

std::ostream& operator<<(std::ostream& os, const glow::Ray& ray);

