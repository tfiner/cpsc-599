// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// Code from Bruce Dawson:
// http://randomascii.wordpress.com/

#include <math.h>


namespace glow {


    bool AlmostEqualUlpsAndAbs(float A, float B,
                float maxDiff, int maxUlpsDiff)
    {
        // Check if the numbers are really close -- needed
        // when comparing numbers near zero.
        float absDiff = fabs(A - B);
        if (absDiff <= maxDiff)
            return true;
     
        Float_t uA(A);
        Float_t uB(B);
     
        // Different signs means they do not match.
        if (uA.Negative() != uB.Negative())
            return false;
     
        // Find the difference in ULPs.
        int ulpsDiff = abs(uA.i - uB.i);
        if (ulpsDiff <= maxUlpsDiff)
            return true;
     
        return false;
    }
     
    bool AlmostEqualRelativeAndAbs(float A, float B,
                float maxDiff, float maxRelDiff)
    {
        // Check if the numbers are really close -- needed
        // when comparing numbers near zero.
        float diff = fabs(A - B);
        if (diff <= maxDiff)
            return true;
     
        A = fabs(A);
        B = fabs(B);
        float largest = (B > A) ? B : A;
     
        if (diff <= largest * maxRelDiff)
            return true;
        return false;
    }


} // namespace glow

