// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// Routines taken from Bruce Dawson's blog.
//
// http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/

namespace glow {


    bool AlmostEqualUlpsAndAbs(float A, float B,
                float maxDiff, int maxUlpsDiff);


    bool AlmostEqualRelativeAndAbs(float A, float B,
                float maxDiff, float maxRelDiff);


} // glow


