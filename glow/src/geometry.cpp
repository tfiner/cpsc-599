// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "geometry.h"
#include <iostream>


using namespace glow;


std::ostream& operator<<(std::ostream& os, const glow::Ray& r) {
    os  << "Ray :\n"
        << "o   :\n" << r.o << "\n"
        << "d   :\n" << r.d << "\n";

    return os;
}
