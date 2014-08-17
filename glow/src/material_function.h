// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "material.h"
#include "hit_record.h"

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <memory>


namespace noise {
    namespace module {
        class Module;
    }
}


namespace glow {


    using KeyValues = std::map<std::string, std::string>;
    using KeyValuesSeq = std::vector<KeyValues>;
    using EvalFunc =  std::function<float(float, float, float)>;
    using NoiseModulePtr = std::shared_ptr<noise::module::Module>;
    using NoiseModules = std::map<std::string, NoiseModulePtr>;


    // Shading based upon a function.
    class MaterialFunction : public Material {
    public:
        MaterialFunction(const KeyValuesSeq& kvs);

        virtual ~MaterialFunction() {}

    private:
        virtual Color ShadeImpl(const HitRecord& hr, const LightPtr&) const override;

        // This sequence owns all of libnoise modules 
        // pointers and is not used for execution.
        NoiseModules modules;

        // All of the functions / modules get collapsed into
        // a single EvalFunc.  
        EvalFunc evalFunc;
    };


} // namespace glow
