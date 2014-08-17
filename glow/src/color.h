// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#pragma once

namespace glow {


    struct Color {
        Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
        Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_), a(1.0f) {}
        float r, g, b, a;
    };


} // namespace glow
