// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "sampler_uniform.h"
#include "log_msg.h"


#include <cmath>

using namespace glow;


// n should really be a power of four, and an assertion enforces this.
Samples SamplerUniform::GenerateNImpl(int n) {
    assert( n > 0 );
    assert( n == 1 || (n%4) == 0 && "N samples for uniform sampler must be a power of 4.");

    Samples samples;
    samples.reserve(n);
    if ( n == 1 ) {
        samples.push_back(Vector3(0.5f, 0.5f, 0.0f));
        return samples;
    }


    /*
        n = 4

           |0.5|
         ---------
         | * | * |
      0  ----+----
         | * | * |
         ---------
             0   


        n = 16

           |0.25/
           |   /        0.5
         ----------------  0.5
         | * | * | * | * |
         ----------------
         | * | * | * | * |
      0  --------+-------
         | * | * | * | * |
         ----------------
         | * | * | * | * | 
    -0.5 ----------------
       -0.5      0

           0   1   2   3
    */

    auto const side = sqrt(n);
    auto const diff = 1.0f / side;
    auto const off = -0.5f + (diff / 2.0f);
    LOG_MSG( 5, "Gen: " << n
        << " side : " << side
        << " diff : " << diff
        << " off : " << off
     );

    for(int y = 0; y < side; ++y) {
        for(int x = 0; x < side; ++x) {
            Vector3 v( x * diff + off, 
                       y * diff + off, 
                      0.0f );
            LOG_MSG( 5, "s:\n" << v );
            samples.push_back(v);
        }
    }

    return samples;
}


SamplerPtr SamplerUniform::CloneImpl() const {
    return SamplerPtr( new SamplerUniform );
}

