// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#include "noise_factory.h"
#include "simplexnoise1234.h"
#include "log_msg.h"

#include "module/module.h"
#include "noiseutils.h"

#include <boost/lexical_cast.hpp>

#include <cassert>
#include <stack>
#include <list>
#include <cmath>
#include <cstdlib>
#include <sstream>


using namespace glow;
using namespace noise::module;
using namespace boost;


namespace {


    // Simple test functions
    float simplex(float x, float y, float) {
        return SimplexNoise1234::noise( x, y );
    }

    float sinxy(float x, float y, float) {
        return sin(x) * sin(y);
    }


    EvalFunc MakeGlowEvalFunc(const KeyValues& kv){
        EvalFunc evalFunc(noop);

        auto const & type = kv.find("type");
        if ( type == kv.end() )
            throw std::runtime_error("Missing type!");

        if ( type->second == "simplex" )
            evalFunc = simplex;
        else if ( type->second == "sinxy" )
            evalFunc = sinxy;
        else 
            throw std::runtime_error(std::string("Unknown function type: '") + type->second + "'");

        return evalFunc;
    }


    // Because I realized the Perlin, Billow and MultiRidged are 
    // essentially the same, but didn't share any inheritance!
    template<typename ModuleType>
    void MakePerlinType(ModuleType& mod, const KeyValues& kv){
        LOG_MSG(3, "Initializing a Perlin type.");

        auto param = kv.find("frequency");
        auto freq = param != kv.end() ? lexical_cast<double>(param->second) : DEFAULT_PERLIN_FREQUENCY;
        mod.SetFrequency( freq );

        param = kv.find("lacunarity");
        auto lacunarity = param != kv.end() ? lexical_cast<double>(param->second) : DEFAULT_PERLIN_LACUNARITY;
        mod.SetLacunarity(lacunarity);

        param = kv.find("octave");
        auto octave = param != kv.end() ? lexical_cast<int>(param->second) : DEFAULT_PERLIN_OCTAVE_COUNT;
        mod.SetOctaveCount(octave);

        param = kv.find("quality");
        auto quality = param != kv.end() ? lexical_cast<int>(param->second) : DEFAULT_PERLIN_QUALITY;
        mod.SetNoiseQuality(static_cast<noise::NoiseQuality>(quality));

        param = kv.find("seed");
        auto seed = param != kv.end() ? lexical_cast<int>(param->second) : DEFAULT_PERLIN_SEED;
        mod.SetSeed(seed);
    }


    NoiseModulePtr MakeRidgedMulti(const KeyValues& kv){
        LOG_MSG(2, "Making a RidgedMulti module.");

        auto multi = new RidgedMulti;
        MakePerlinType(*multi, kv);

        return NoiseModulePtr(multi);        
    }


    NoiseModulePtr MakeBillow(const KeyValues& kv){
        LOG_MSG(2, "Making a Billow module.");

        auto billow = new Billow;
        MakePerlinType(*billow, kv);

        auto param = kv.find("persistence");
        auto persistence = param != kv.end() ? 
            lexical_cast<double>(param->second) : 
            DEFAULT_BILLOW_PERSISTENCE;

        billow->SetPersistence(persistence);

        return NoiseModulePtr(billow);        
    }


    NoiseModulePtr MakePerlin(const KeyValues& kv){
        LOG_MSG(2, "Making a Perlin module.");

        auto perlin = new Perlin;
        MakePerlinType(*perlin, kv);

        auto param = kv.find("persistence");
        auto persistence = param != kv.end() ? 
            lexical_cast<double>(param->second) : 
            DEFAULT_PERLIN_PERSISTENCE;

        LOG_MSG(2, "persistence: " << persistence);
        perlin->SetPersistence(persistence);

        return NoiseModulePtr(perlin);        
    }


    NoiseModulePtr MakeScale(const KeyValues& kv){
        LOG_MSG(2, "Making a scale module.");

        auto scaleBias = new ScaleBias;
        auto param = kv.find("scale");
        auto scale = param != kv.end() ? lexical_cast<double>(param->second) : DEFAULT_SCALE;
        scaleBias->SetScale(scale);

        param = kv.find("bias");
        auto bias = param != kv.end() ? lexical_cast<double>(param->second) : DEFAULT_BIAS;
        scaleBias->SetBias(bias);

        return NoiseModulePtr(scaleBias);        
    }


    NoiseModulePtr MakeSelect(const KeyValues& kv){
        LOG_MSG(2, "Making a select module.");

        auto select = new Select;
        auto param = kv.find("edgeFalloff");
        auto falloff = param != kv.end() ? 
            lexical_cast<double>(param->second) : 
            DEFAULT_SELECT_EDGE_FALLOFF;
        select->SetEdgeFalloff(falloff);

        param = kv.find("lower");
        auto lower = param != kv.end() ? 
            lexical_cast<double>(param->second) : 
            DEFAULT_SELECT_LOWER_BOUND;

        param = kv.find("upper");
        auto upper = param != kv.end() ? 
            lexical_cast<double>(param->second) : 
            DEFAULT_SELECT_UPPER_BOUND;

        select->SetBounds(lower, upper);

        return NoiseModulePtr(select);        
    }


    NoiseModulePtr MakeLibNoiseModule(const KeyValues& kv) {
        auto const & type = kv.at("type");
        auto const & name = kv.at("name");

        LOG_MSG(2, "Making libnoise module '" << name << "'");

        if ( type == "checkerboard" )
            return NoiseModulePtr( std::make_shared<Checkerboard>() );

        else if ( type == "perlin" )
            return MakePerlin(kv);

        else if ( type == "billow" )
            return MakeBillow(kv);

        else if ( type == "ridgedmulti" )
            return MakeRidgedMulti(kv);

        else if ( type == "scale" )
            return MakeScale(kv);

        else if ( type == "select" )
            return MakeSelect(kv);

        else if ( type == "spheres" )
            return NoiseModulePtr( std::make_shared<Spheres>() );

        else
            assert(!"Forgot a noise module type, dummy?");

        return nullptr;
    }


    std::string GetSourceName(int index) {
        std::stringstream ss;
        ss << "source" << index;
        return ss.str();
    }


    NoiseModulePtr FindSrcModule(
            NoiseModules& modules, 
            const KeyValues& kv, 
            std::string name) {

        auto kvSrc = kv.find(name);
        if (kvSrc == kv.end()) 
            return nullptr;
        
        auto srcMod = modules.find(kvSrc->second);
        return srcMod != modules.end() ? srcMod->second: nullptr;
    }


    void CreateLibNoise(
        std::list<NoiseModulePtr>& moduleList, 
        NoiseModules& modules, 
        const KeyValues& kv) {

        auto mod = MakeLibNoiseModule(kv);
        auto const name = kv.at("name");
        modules.insert( std::make_pair(name,mod) );
        moduleList.push_front(mod);

        auto srcModCount = mod->GetSourceModuleCount(); 
        if (srcModCount < 0)
            throw std::runtime_error("Source Module Count < 0!");

        auto srcMod = FindSrcModule(modules, kv, "control");
        if (srcMod){
            LOG_MSG(2, 
                typeid(*mod).name() 
                << " adding " 
                << typeid(*srcMod).name() );

            moduleList.remove(srcMod);
            if ( auto select = dynamic_cast<Select*>(mod.get()) )
                select->SetControlModule(*srcMod);
            else if ( auto blend = dynamic_cast<Blend*>(mod.get()) )
                blend->SetControlModule(*srcMod);
            else {
                throw std::runtime_error("Unknown module with control: " + name);                
            }
            srcModCount--;
        }

        for( int i = 0; i < srcModCount; ++i ){
            auto srcKey = GetSourceName(i);
            auto srcMod = FindSrcModule(modules, kv, srcKey);
            if (srcMod){
                LOG_MSG(2, 
                    typeid(*mod).name() 
                    << " adding " 
                    << typeid(*srcMod).name() );

                moduleList.remove(srcMod);
                mod->SetSourceModule(i, *srcMod);
            } else
                throw std::runtime_error(std::string("Couldn't find ") + srcKey);            
        }
    }


    void WriteTestBmp(Module& module){
        utils::NoiseMap heightMap;
        utils::NoiseMapBuilderPlane heightMapBuilder;
        heightMapBuilder.SetSourceModule (module);
        heightMapBuilder.SetDestNoiseMap (heightMap);
        heightMapBuilder.SetDestSize (256, 256);
        heightMapBuilder.SetBounds (0.0, 20.0, 0.0, 20.0);
        heightMapBuilder.Build ();

        utils::RendererImage renderer;
        utils::Image image;
        renderer.SetSourceNoiseMap (heightMap);
        renderer.SetDestImage (image);
        renderer.Render ();

        utils::WriterBMP writer;
        writer.SetSourceImage (image);
        writer.SetDestFilename ("map.bmp");
        writer.WriteDestFile ();        
    }


} // namespace {


// Given a sequence of noise functions,
// combine them into an single evaluation function.
EvalFunc glow::CreateFunctions( NoiseModules& modules, const KeyValuesSeq& kvs ) {
    std::list<NoiseModulePtr> moduleList;
    std::stack<EvalFunc> evalFuncStack;

    EvalFunc composition(noop);

    for( auto const & kv : kvs ) {
        auto const & type = kv.find("type");
        if ( type == kv.end() )
            throw std::runtime_error("Missing function type!");

        LOG_MSG(2, "type: " << type->second);

        auto const & lib = kv.find("lib");
        if ( lib == kv.end() )
            throw std::runtime_error("Missing function lib!");

        LOG_MSG(2, "lib: " << lib->second);

        if ( lib->second == "libnoise" ) {
            CreateLibNoise(moduleList, modules, kv);

        } else if ( lib->second == "glow" ) {
            auto func = MakeGlowEvalFunc(kv);
            evalFuncStack.push(func);

        } else {
            throw std::runtime_error("Unknown lib!");
        }
    }

    // If there are any modules, there should only be one left.
    if ( !moduleList.empty() ) {
        if ( moduleList.size() != 1 )
            throw std::runtime_error("Disjoint noise tree - all noise modules should collapse to one module!");            
        
        auto & module = moduleList.front();

        // There is always a pointer to the libnoise function owned by modules.
        // The lambda below binds a reference to this pointer.

        LOG_MSG(2, "Binding module (" << module.get() << ") " 
            << typeid(*module).name() << " to eval function.");

        // Note that the module pointer is _shared_ and is copied by value.
        // This increases the reference count by one, and follows the copies
        // of the composition function around.
        // Libnoise module takes precedence over any glow eval functions.
        composition = [module](float x, float y, float z)->float{
            return module->GetValue(x,y,z);
        };

        WriteTestBmp(*moduleList.front());

    } // else look for glow functions

    assert( evalFuncStack.empty() && "Not implemented yet." );

    return composition;
}

