// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This is the scene parser, it uses the boost spirit parser.
//

#include "memento.h"
#include "log_msg.h"

#include <iostream>
#include <stdexcept>

#include <json/elements.h>
#include <json/reader.h>
#include <json/writer.h>


using namespace std;
using namespace glow;


namespace {


    struct MissingElement : public std::runtime_error {
        MissingElement(const char* what) : std::runtime_error(what) {}
    };


    template<typename NumberType>
    inline void LoadNumber(NumberType& n, const json::Number& jsonObj) {
        n = jsonObj.Value();
    }

    inline void LoadString(std::string& s, const json::String& jsonObj) {
        s = jsonObj.Value();
    }


    void LoadVector(VectorParams& v, json::Object jsonObj) {
        LoadNumber( v.x, jsonObj["x"] );
        LoadNumber( v.y, jsonObj["y"] );
        LoadNumber( v.z, jsonObj["z"] );
    }


    // I'm lazy and treat colors as vectors.
    void LoadColor(VectorParams& v, json::Object jsonObj) {
        LoadNumber( v.x, jsonObj["r"] );
        LoadNumber( v.y, jsonObj["g"] );
        LoadNumber( v.z, jsonObj["b"] );
    }


    void LoadCamera(SceneParams& sp, json::Object& jsonObj){
        /*
            "camera" : {
                "type":"SimplePerspectiveCamera",
                "center": {
                    "x": 1,
                    "y": 0,
                    "z": 0.5
                },
                "direction": {
                    "x": 1,
                    "y": 0,
                    "z": 0.5
                },
                "up": {
                    "x": 1,
                    "y": 0,
                    "z": 0.5
                },
                "distance": 5.0                  
            }
        */
        auto const & camera = (json::Object)(jsonObj["camera"]);
        LoadString( sp.camera.type,         camera["type"] );
        LoadVector( sp.camera.position,     camera["center"] );
        LoadVector( sp.camera.direction,    camera["direction"] );

        if ( camera.End() != camera.Find("up") )
            LoadVector( sp.camera.up,       camera["up"] );
        else
            LOG_MSG(2, "Didn't find a camera up vector...");

        if ( camera.End() != camera.Find("right") )
            LoadVector( sp.camera.right,    camera["right"] );
        else
            LOG_MSG(2, "Didn't find a camera right vector...");

        LoadNumber( sp.camera.distance,     camera["distance"] );
    }


    void LoadViewPlane(SceneParams& sp, json::Object& jsonObj){
        /*
            "viewPlane": {
                "width":        7,
                "height":       7,
                "pixelSize":    0.0125
            }
        */
        auto const & vp = (json::Object)(jsonObj["viewPlane"]);
        LoadNumber( sp.viewPlane.width,     vp["width"] );
        LoadNumber( sp.viewPlane.height,    vp["height"] );
        LoadNumber( sp.viewPlane.pixelSize, vp["pixelSize"] );
    }


    void LoadMaterial(MaterialParams& mp, const json::Object& jsonObj){
        /*
            "type":"phong",
            "diffuseColor":  { "r":0.0, "g":0.5, "b":1.0 },
            "specularColor": { "r":1.0, "g":1.0, "b":1.0 },
            "exponent": 64.0
        */
        LoadString( mp.type,        jsonObj["type"] );
        
        if ( jsonObj.End() != jsonObj.Find("diffuseColor") )
            LoadColor( mp.diffuse, jsonObj["diffuseColor"] );

        if ( jsonObj.End() != jsonObj.Find("specularColor") )
            LoadColor( mp.specular, jsonObj["specularColor"] );

        if ( jsonObj.End() != jsonObj.Find("exponent") )
            LoadNumber( mp.exponent,    jsonObj["exponent"] );
    }


    void LoadMaterials(SceneParams& sp, json::Object& jsonObj){
        /*
        "materials": [
            {
                "type":"phong",
                "diffuseColor":  { "r":0.0, "g":0.5, "b":1.0 },
                "specularColor": { "r":1.0, "g":1.0, "b":1.0 },
                "exponent": 64.0
            }
        ]
        */
        auto const & mats = (json::Array)(jsonObj["materials"]);
        for ( size_t i = 0; i < mats.Size(); ++i ) {
            MaterialParams mp;
            LoadMaterial( mp, (const json::Object &)mats[i] );
            sp.materials.push_back(mp);
        }
    }


    void LoadKeyVals( KeyValues& kvs, const json::Object& jsonObj ) {
        assert( !jsonObj.Empty() );

        using namespace json;
        // std::stringstream ss;
        // Writer::Write(jsonObj, ss);
        // std::cout << "jsonObj:\n" << ss.str() << "\n";

        for( auto iter = jsonObj.Begin(); iter != jsonObj.End(); ++iter ){
            const Object::Member& member = *iter;
            const std::string& name = member.name;
            const UnknownElement& element = member.element;
            LOG_MSG(3, "name: " << name);
            auto value = ((const json::String &)element).Value();
            LOG_MSG(3, "value: " << value);
            kvs.insert( std::make_pair(name, value) );
        }
    }


    ObjectParamsPtr LoadFunctionObject(const json::Object& jsonObj, int matIdx){
        auto const & funcs = (json::Array)(jsonObj["functions"]);

        std::vector< KeyValues > kvs;
        for ( size_t i = 0; i < funcs.Size(); ++i ) {
            KeyValues kv;
            LoadKeyVals( kv, (const json::Object &)(funcs[i]) );
            kvs.push_back( kv );
        }
        return new FunctionParams(matIdx, kvs); 
    }
    

    ObjectParamsPtr LoadPlane(const json::Object& jsonObj, int matIdx){
        PlaneParams* p = new PlaneParams;
        p->materialIndex = matIdx;
        p->type = "plane";
        LoadVector(p->normal, jsonObj["normal"]);
        LoadNumber(p->offset, jsonObj["offset"]);
        return p; 
    }


    ObjectParamsPtr LoadObject(const json::Object& jsonObj){
        /*
            "materialIdx":0,
            "type":"function",
            "functions":[
                { "type":"simplexNoise1234" }
            ]
        */
        ObjectParamsPtr op = nullptr;
        std::string type;
        LoadString( type, jsonObj["type"] );

        int matIdx = -1;
        LoadNumber(matIdx, jsonObj["materialIdx"]);

        if ( type == "function" )
            op = LoadFunctionObject(jsonObj, matIdx);
        else if ( type == "plane" )
            op = LoadPlane(jsonObj, matIdx);
        else
            assert(!"Unknown object!");

        return op;
    }


    void LoadObjects(SceneParams& sp, json::Object& jsonObj){
        /*
        "objects": [
            { 
            }
        ]
        */
        auto const & objs = (json::Array)(jsonObj["objects"]);
        for ( size_t i = 0; i < objs.Size(); ++i ) {
            ObjectParamsPtr op = LoadObject( (const json::Object &)objs[i] );
            sp.objects.push_back(op);
        }
    }


    void LoadLight(LightParams& light, const json::Object& jsonObj){
        /*
            "type":"DirectionalLight",
            "position":     { "x": 0.0, "y": 0.0, "z": 0.0 },
            "direction":    { "x":-1.0, "y": 0.0, "z": 1.0 }
            "color":        { "r": 1.0, "g": 1.0, "b": 1.0 }
            "attenuation":  { "r": 0.1, "g": 0.1, "b": 0.1 }
        */
        LoadString( light.type,         jsonObj["type"] );

        if ( jsonObj.End() != jsonObj.Find("direction") )
            LoadVector( light.direction,    jsonObj["direction"] );

        if ( jsonObj.End() != jsonObj.Find("position") )         
             LoadVector( light.position,     jsonObj["position"] );             

        LoadColor(  light.color,        jsonObj["color"] );
        LoadColor(  light.intensity,    jsonObj["attenuation"] );
    }


    void LoadLights(SceneParams& sp, json::Object& jsonObj){
        /*
        "lights": [
            { 
            }
        ]
        */
        auto const & lights = (json::Array)(jsonObj["lights"]);
        for ( size_t i = 0; i < lights.Size(); ++i ) {
            LightParams lp;
            LoadLight( lp, (const json::Object &)lights[i] );
            sp.lights.push_back(lp);
        }
    }

    void LoadSceneSampler(SceneParams& sp, const json::Object& jsonObj){
        LoadKeyVals( sp.sceneSampler.keyVals, jsonObj["sceneSampler"] );
    }


    void LoadFog(FogParams& fp, const json::Object& jsonObj) {
        if ( jsonObj.End() != jsonObj.Find("color") )
            LoadColor( fp.color, jsonObj["color"] );

        if ( jsonObj.End() != jsonObj.Find("density") )
            LoadNumber( fp.density, jsonObj["density"] );

        if ( jsonObj.End() != jsonObj.Find("maximum") )
            LoadNumber( fp.maximum, jsonObj["maximum"] );
    }

    void LoadBackgroundSampler(SceneParams& sp, const json::Object& jsonObj){
        LoadKeyVals( sp.backgroundSampler, jsonObj );
    }


} // namespace {


namespace glow {


    SceneParams LoadScene(std::string& sceneTxt) {
        stringstream ss;
        ss << sceneTxt;
        json::Object root;
        try {
            json::Reader::Read(root, ss);

        } catch(const json::Reader::ScanException& e) {
            cerr << "Scan exception at " 
                 << "line " << e.m_locError.m_nLine + 1 << " "
                 << "char " << e.m_locError.m_nLineOffset + 1 << "\n";
            throw;

        } catch(const json::Reader::ParseException& e) {
            cerr << "Parse exception between:\n" 
                 << " line " << e.m_locTokenBegin.m_nLine + 1 << " "
                 << " char " << e.m_locTokenBegin.m_nLineOffset + 1 << " "
                 << " line " << e.m_locTokenEnd.m_nLine + 1 << " "
                 << " char " << e.m_locTokenEnd.m_nLineOffset + 1 << "\n";
            throw;
        }

        SceneParams scene;
        LoadCamera(scene, root);
        LoadViewPlane(scene, root);
        LoadColor( scene.background, root["backgroundColor"] );
        LoadColor( scene.ambient, root["ambientColor"] );
        LoadMaterials(scene, root);
        LoadObjects(scene, root);
        LoadLights(scene, root);
        LoadSceneSampler(scene, root);
        if ( root.End() != root.Find("fog") )
            LoadFog(scene.fogParams, root["fog"]);

        if ( root.End() != root.Find("backgroundSampler") )
            LoadBackgroundSampler(scene, root["backgroundSampler"]);

        return scene;
    }


} // namespace glow
