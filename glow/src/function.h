// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//


#pragma once


#include "object.h"
#include "geometry.h"

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


    struct HitRecord;
    struct Ray;

    using KeyValues = std::map<std::string, std::string>;
    using KeyValuesSeq = std::vector<KeyValues>;
    using EvalFunc =  std::function<float(float, float, float)>;
    using NoiseModulePtr = std::shared_ptr<noise::module::Module>;
    using NoiseModules = std::map<std::string, NoiseModulePtr>;

    // An object that can be represented by a function.
    class Function : public Object {
    public:
        Function(int matIdx, const KeyValuesSeq& kvs);

        const NoiseModules& GetModules() const { return modules; }

    private:
        bool HitImpl( HitRecord& hr, const Ray& r ) const override;
        bool InsideImpl( const Point3& p ) const override;

        Vector3 GetNormal(const Point3& pt) const override;

        // This sequence owns all of libnoise modules 
        // pointers and is not used for execution.
        NoiseModules modules;

        // All of the functions / modules get collapsed into
        // a single EvalFunc.  
        EvalFunc evalFunc;
    };


} // namespace glow
