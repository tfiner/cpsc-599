// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#include "sampler.h"


namespace glow {


    // Generates uniform samples across the square of x:[0,1] and y:[0,1]
    class SamplerUniform : public Sampler {
    public:
    private:
        // n should really be a power of four, and an assertion enforces this.
        virtual Samples GenerateNImpl(int n) override;

        virtual SamplerPtr CloneImpl() const override;
    };


} // namespace glow
