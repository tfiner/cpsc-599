// Tim Finer 
// tfiner@csu.fullerton.edu
// 
// This file contains all of the serialization mementos to create objects.
// Each memento contains enough to pick a subclass via a "type" parameter.
// 
// The rationale for this is that these classes contain all of the parameters
// needed for a factory to create an instance of an object.  This was 
// decoupled, so that the serialization can vary from the factory without 
// any changes.

#pragma once


#include <iosfwd>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <boost/any.hpp>


namespace glow {

  
    struct VectorParams {
        VectorParams() : x(0.0), y(0.0), z(0.0) {}
        VectorParams(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
        float x, y, z;
    };

    struct CameraParams {
        CameraParams() : fov(0.0f), distance(1.0f) {}

        CameraParams(std::string t, 
            const VectorParams& pos, const VectorParams& dir, const VectorParams& up_ ) : 
            type(t), position(pos), direction(dir), up(up_), fov(0.0f), distance(1.0f) {}

        CameraParams(std::string t, 
            const VectorParams& pos, const VectorParams& dir, const VectorParams& up_,
            float fov_, float dist ) : 
            type(t), position(pos), direction(dir), up(up_), fov(fov_), distance(dist) {}

        std::string type;
        VectorParams position;
        VectorParams direction;

        // Either one of these is set.
        VectorParams up;
        VectorParams right;
        
        float fov;
        float distance;
    };

    struct ViewPlaneParams {
        ViewPlaneParams() : x0(0), x1(0), y0(0), y1(0), pixelSize(0.0f) {}
        int x0, x1;
        int y0, y1;
        float pixelSize;
    };

    struct MaterialParams {
        MaterialParams() {}
        
        MaterialParams(std::string t, const VectorParams& diffuse_) : 
            type(t), diffuse(diffuse_), exponent(1.0f) {}

        MaterialParams(std::string t, 
            const VectorParams& diffuse_, 
            const VectorParams& specular_, 
            float exponent_) : 
                type(t), diffuse(diffuse_), specular(specular_), exponent(exponent_) {}
        
        std::string type;
        VectorParams diffuse;
        VectorParams specular;
        float exponent;
    };

    typedef std::vector<MaterialParams> MaterialParamsSeq;


    struct ObjectParams {
        ObjectParams() : materialIndex(-1) {}
        ObjectParams(std::string t, int mi) : type(t), materialIndex(mi) {}
        ObjectParams(std::string t) : type(t), materialIndex(-1) {}
        virtual ~ObjectParams(){}
        std::string type;
        int materialIndex;
    };

    using KeyValues = std::map<std::string, std::string>;
    using KeyValuesSeq = std::vector<KeyValues>;

    // Completely generic name, followed by a list of key value pairs.
    // TODO: This is the future of all these params and all of them 
    // should be reduced to this.
    struct FunctionParams : public ObjectParams {
        FunctionParams(int matIdx, KeyValuesSeq& kvs) : 
            ObjectParams("function", matIdx), keyVals(kvs) {}

        FunctionParams() : ObjectParams("function", -1) {}

        KeyValuesSeq keyVals;
    };

    // Sooo much simpler.
    struct SceneSamplerParams {
        KeyValues keyVals;
    };


    struct SphereParams : public ObjectParams {
        SphereParams() : ObjectParams("sphere", -1), radius(0.0f) {}
        SphereParams(VectorParams center_, float radius_, int matIdx);
        VectorParams center;
        float radius;
    };

    struct PlaneParams : public ObjectParams {
        PlaneParams() : ObjectParams("plane", -1) {}

        PlaneParams(VectorParams normal_, float offset_, int matIdx) : 
            ObjectParams("plane", matIdx), normal(normal_), offset(offset_) {}

        VectorParams normal;
        float offset;
    };


    typedef ObjectParams* ObjectParamsPtr;
    typedef std::vector<ObjectParamsPtr> ObjectParamsSeq;


    struct CompositeParams : public ObjectParams {
        CompositeParams(int matIdx, ObjectParamsSeq& objs);
        ObjectParamsSeq objects;        
    };


    struct LightParams {
        LightParams() {}

        LightParams(std::string t, 
            const VectorParams& pos, 
            const VectorParams& dir,
            const VectorParams& color_,
            const VectorParams& i ) :
            type(t), position(pos), color(color_), direction(dir), intensity(i) {}

        std::string type;
        VectorParams position;
        VectorParams color;
        VectorParams direction;
        VectorParams intensity;
    };

    typedef std::vector<LightParams> LightParamsSeq;

    struct FogParams {
        FogParams(): color(0,0,0), density(0.0f), maximum(0) {}
        VectorParams color;
        float density;
        float maximum;
    };

    struct SceneParams {
        CameraParams camera;
        ViewPlaneParams viewPlane;
        VectorParams background;
        VectorParams ambient;
        MaterialParamsSeq materials;
        ObjectParamsSeq objects;
        LightParamsSeq lights;
        SceneSamplerParams sceneSampler;
        FogParams fogParams;
        KeyValues backgroundSampler;
    };

};


// These are mostly for debugging purposes to verify that the objects 
// were serialized properly.  Also, it provides a way for unit tests
// to report errors.
std::ostream& operator<<(std::ostream& os, const glow::VectorParams& vp);
std::ostream& operator<<(std::ostream& os, const glow::LightParams& lp);
std::ostream& operator<<(std::ostream& os, const glow::CameraParams& cp);
std::ostream& operator<<(std::ostream& os, const glow::ViewPlaneParams& vpp);
std::ostream& operator<<(std::ostream& os, const glow::MaterialParams& mp);
std::ostream& operator<<(std::ostream& os, const glow::ObjectParams& sp);
std::ostream& operator<<(std::ostream& os, const glow::FunctionParams& fp);
std::ostream& operator<<(std::ostream& os, const glow::SphereParams& sp);
std::ostream& operator<<(std::ostream& os, const glow::PlaneParams& sp);
std::ostream& operator<<(std::ostream& os, const glow::CompositeParams& sp);
std::ostream& operator<<(std::ostream& os, const glow::SceneSamplerParams& ssp);
std::ostream& operator<<(std::ostream& os, const glow::FogParams& fp);
std::ostream& operator<<(std::ostream& os, const glow::SceneParams& sp);


