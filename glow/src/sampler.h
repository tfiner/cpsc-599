// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#include "geometry.h"
#include <vector>
#include <memory>


namespace glow {


    using Samples = std::vector<Vector3>;
    class Sampler;
    using SamplerPtr = std::unique_ptr<Sampler>;

    // A generic sampler.
    class Sampler {
    public:
        virtual ~Sampler(){}

        // Fills in samples argument with n samples that are [-0.5,0.5) for x,y,z.
        Samples GenerateN(int n) { return GenerateNImpl(n); }

        // Returns a clone of this sampler type.
        SamplerPtr Clone() const { return CloneImpl(); }

    private:
        virtual Samples GenerateNImpl(int n) = 0;
        virtual SamplerPtr CloneImpl() const = 0;
    };


} // namespace glow
