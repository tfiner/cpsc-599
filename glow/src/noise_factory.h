// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//


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


    // The default glow EvalFunc.
    inline float noop(float, float, float) { return 0.0f; }


    using KeyValues = std::map<std::string, std::string>;
    using KeyValuesSeq = std::vector<KeyValues>;
    using EvalFunc =  std::function<float(float, float, float)>;

    // Take care of libnoise Module allocation/deallocation.
    using NoiseModulePtr = std::shared_ptr<noise::module::Module>;
    using NoiseModules = std::map<std::string, NoiseModulePtr>;


    // This function has two purposes:
    //
    // 1. Given a sequence of noise functions in postfix order,
    // combine them into an single evaluation function.
    //
    // 2. Push all libnoise Modules into modules to take care of Module
    // pointer ownership.  EvalFuncs can bind them in closures without
    // needing to worry about their lifetime (because of shared_ptr).
    EvalFunc CreateFunctions( NoiseModules& modules, const KeyValuesSeq& kvs );

    // Writes preview BMPs to a file.
    void WritePreviewBmps(const NoiseModules& modules, std::string path);

}
