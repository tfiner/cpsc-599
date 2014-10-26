// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file contains the factory for all of the raytracing objects.

#include "factory.h"
#include "memento.h"
#include "scene.h"

#include "camera.h"
#include "orthogonal_camera.h"
#include "perspective_camera.h"
#include "simple_perspective_camera.h"

#include "view_plane.h"

#include "material.h"
#include "matte.h"
#include "phong.h"
#include "constant_shader.h"
#include "material_gradient.h"

#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "function.h"

#include "recorder_png.h"

#include "direction_light.h"
#include "point_light.h"
#include "ambient_occluder_light.h"

#include "sampler_uniform.h"
#include "scene_sampler.h"

#include "background_sampler.h"
#include "starry_night.h"

#include "log_msg.h"

#include <cassert>


using namespace glow;


namespace {


    const double PI             = 3.14159265359;
    const double PI_OVER_180    = PI / 180.0;


    inline float DegreesToRadians(float degrees) {
        return degrees * PI_OVER_180;
    }


    Point3 MakePoint(const VectorParams& vp) {
        return Point3( vp.x, vp.y, vp.z );
    }


    Vector3 MakeVector(const VectorParams& vp) {
        return Vector3( vp.x, vp.y, vp.z );
    }


    Color MakeColor(const VectorParams& vp) {
        return Color( vp.x, vp.y, vp.z );
    }


    CameraPtr MakeCamera(const SceneParams& sp) {
        auto const& cp = sp.camera;
        auto pos = MakePoint(cp.position);
        auto dir = MakeVector(cp.direction).normalized();
        LOG_MSG(2, "Camera dir:\n" << dir);

        auto up = MakeVector(cp.up);
        if ( up.squaredNorm() == 0.0f ) {
            auto right = MakeVector(cp.right);
            LOG_MSG(2, "Camera up vector 0, using right:\n" << right);

            assert( right.squaredNorm() != 0.0f );
            up = right.normalized().cross(dir);
            LOG_MSG(2, "Camera up:\n" << up);

        }
        up = up.normalized();

        if ( cp.type == "orthogonal" ){
            return CameraPtr( 
                new OrthogonalCamera(pos, dir, up)
            );

        } else if ( cp.type == "perspective" ) {
            auto radians = DegreesToRadians(cp.fov); 
            return CameraPtr( 
                new PerspectiveCamera(pos, dir, up, radians)
            );

        } else if ( cp.type == "SimplePerspective" ) {
            return CameraPtr( 
                new SimplePerspectiveCamera(pos, dir, up, cp.distance)
            );
        }

        assert(!"Unknown camera type!");
        return nullptr;
    }


    ViewPlanePtr MakeViewPlane(const SceneParams& sp) {
        auto const& vp = sp.viewPlane;
        return ViewPlanePtr(new ViewPlane(vp.x0, vp.x1, vp.y0, vp.y1, vp.pixelSize));
    }


    MaterialSeq MakeMaterials(const SceneParams& sp) {
        MaterialSeq materials;

        for ( auto const & mp : sp.materials ) {
            if ( mp.type == "phong" ) {
                auto diffuse = MakeColor(mp.diffuse);
                LOG_MSG(3, "diffuse:  " << "r " << diffuse.r << " g " << diffuse.g << " b " << diffuse.b );

                auto specular = MakeColor(mp.specular);
                LOG_MSG(3, "specular: " << "r " << specular.r << " g " << specular.g << " b " << specular.b );

                materials.push_back( MaterialPtr(new Phong(diffuse, specular, mp.exponent)) );

            } else if ( mp.type == "matte" ) {
                auto diffuse = MakeColor(mp.diffuse);
                LOG_MSG(3, "diffuse:  " << "r " << diffuse.r << " g " << diffuse.g << " b " << diffuse.b );

                materials.push_back( MaterialPtr(new Matte(diffuse)) );

            } else if ( mp.type == "gradient") {
                materials.push_back( MaterialPtr(new MaterialGradient) );

            } else {
                assert(!"Unknown material type.");
            }
        }

        return materials;
    }


    ObjectPtr MakeSphere(const SphereParams* sphere) {
        return ObjectPtr( 
            new Sphere(
                MakePoint(sphere->center), 
                sphere->radius, 
                sphere->materialIndex
            )
        ); 
    }


    ObjectPtr MakePlane(const PlaneParams* plane) {
        Vector3 norm = MakeVector(plane->normal);
        auto pt = Point3(0,0,0) + norm * plane->offset;

        return ObjectPtr( 
            new Plane(norm, pt, plane->materialIndex)
        ); 
    }


    ObjectPtr MakeFunction(const FunctionParams* fp) {
        LOG_MSG(3, "MakeFunction: begin" );
        auto func = new Function(fp->materialIndex, fp->keyVals);
        return ObjectPtr( func ); 
    }


    void AppendObject(ObjectSeq& objects, const ObjectParams* op) {
        LOG_MSG(3, "object: begin " << op->type );

        if ( auto sphere = dynamic_cast<const SphereParams*>(op) ) {
            assert( op->type == "sphere" );
            objects.push_back( MakeSphere(sphere) );

        } else if ( auto plane = dynamic_cast<const PlaneParams*>(op) ) {
            assert( op->type == "plane" );
            objects.push_back( MakePlane(plane) );

        } else if ( auto comp = dynamic_cast<const CompositeParams*>(op) ) {
            assert( op->type == "composite" );
            for ( auto const& sub : comp->objects )
                AppendObject( objects, sub );

        } else if ( auto fp = dynamic_cast<const FunctionParams*>(op) ) {
            assert( op->type == "function" );
            objects.push_back( MakeFunction(fp) );

        } else {
            assert( !"Unknown object type." );
            throw std::runtime_error("Unknown object type.");
        }

        LOG_MSG(3, "object: end " << op->type );
    }

    ObjectSeq MakeObjects(const SceneParams& sp) {
        ObjectSeq objects;

        for ( auto const & op : sp.objects )
            AppendObject(objects, op);

        return objects;
    }


    LightPtr MakeLight(const LightParams& lp){
        if ( lp.type == "DirectionalLight" ){
            return LightPtr( 
                new DirectionLight(
                    MakeColor(lp.color), 
                    MakeVector(lp.intensity), 
                    MakeVector(lp.direction)
                )
            );

        } else if ( lp.type == "PointLight" ){
            return LightPtr( 
                new PointLight(
                    MakeColor(lp.color), 
                    MakeVector(lp.intensity),
                    MakePoint(lp.position) 
                )
            );
        } else if ( lp.type == "AmbientOccluder" ){
            return LightPtr( 
                new AmbientOccluderLight(
                    MakeColor(lp.color), 
                    MakeVector(lp.intensity) 
                )
            );
        }

        assert(!"Unknown light type!");
        return nullptr;
    }


    LightSeq MakeLights(const SceneParams& sp){
        LightSeq lights;
        for ( auto const & lp : sp.lights ){
            auto light = MakeLight(lp);
            assert( light != nullptr );
            lights.push_back( LightPtr(light.release()) );
        }

        // These have to be empty or have items:
        assert( (sp.lights.empty() && lights.empty()) ||
                (!sp.lights.empty() && !lights.empty()) );

        return lights;
    }


    template<typename T>
    T GetOptionalValue(const KeyValues& kv, const char* key, const char* defaultValue) {
        std::string value(defaultValue);
        auto const & val = kv.find(key);
        if ( val != kv.end() )
            value = val->second;
        return boost::lexical_cast<T>(value);
    }


    SceneSamplerPtr MakeRayMarcher(const SceneParams& scene){
        LOG_MSG(2, "Making RayMarcher:");

        auto & kv = scene.sceneSampler.keyVals;
        auto minDist = GetOptionalValue<float>(kv, "minDistance", "0.0");
        auto maxDist = GetOptionalValue<float>(kv, "maxDistance", "1.0");

        auto const & cam = scene.camera;
        auto farAlong  = MakeVector(cam.direction).normalized() * maxDist;
        auto farPos    = MakePoint(cam.position) + farAlong;
        auto farNormal = -farAlong.normalized();

        LOG_MSG(2, "ignoring minDist:\n" << minDist << "\n");
        LOG_MSG(2, "farPos:\n" << farPos << "\n");
        LOG_MSG(2, "farNormal:\n" << farNormal << "\n");

        Plane farPlane(
            farPos, 
            farNormal,
            -1 // no material
        );

        auto delta = GetOptionalValue<float>(kv, "delta", "0.125");
        return SceneSamplerPtr(new RayMarcher(farPlane, delta));
    }


    SceneSamplerPtr MakeSceneSampler(const SceneParams& scene) {
        auto & kv = scene.sceneSampler.keyVals;
        auto const & type = kv.find("type");
        if ( type == kv.end() )
            throw std::runtime_error("Missing scene sampler type!");

        SceneSamplerPtr sampler;
        if (type->second == "rayMarcher") {
            sampler = MakeRayMarcher(scene);

        } else {
            assert( !"I haven't added raytracer back yet, if that's what you're thinking..." );
            throw std::runtime_error("Unknown scene sampler type.");
        }

        return sampler;
    }


    BackgroundSamplerPtr MakeBackgroundSampler(const KeyValues& kv){
        BackgroundSamplerPtr sampler(new NullBackground);

        if (!kv.empty()){
            auto const & type = kv.find("type");
            if ( type == kv.end() )
                throw std::runtime_error("Missing background sampler type!");

            if ( type->second == "stars"){
                auto density = GetOptionalValue<float>(kv, "density", "0.01");
                auto minLevel = GetOptionalValue<float>(kv, "minLevel", "0.3");
                auto maxLevel = GetOptionalValue<float>(kv, "maxLevel", "0.9");

                sampler.reset( new StarryNight(density, minLevel, maxLevel) );
            }else
                throw std::runtime_error("Unknown background sampler type!");                
        }

        return sampler;
    }


} // namespace {


namespace glow {


    ScenePtr MakeScene( const SceneParams& p ) {
        ScenePtr scene(new Scene);
        scene->camera        = MakeCamera(p);
        scene->viewPlane     = MakeViewPlane(p);
        scene->background    = MakeColor( p.background );
        scene->ambient       = MakeColor( p.ambient );
        scene->materials     = MakeMaterials( p );

        LOG_MSG(3, "MakeObjects" );
        scene->objects       = MakeObjects( p );

        LOG_MSG(3, "MakeLights" );
        scene->lights        = MakeLights( p );

        // TODO: Move this into the scene file.
        // Since I only have one in here, I'll add that feature 
        // when I add more samplers.
        scene->numSamples = 1;
        scene->sampler.reset( new SamplerUniform );

        scene->sceneSampler = MakeSceneSampler(p);

        scene->backgroundSampler = MakeBackgroundSampler(p.backgroundSampler);

        scene->fogColor     = MakeColor(p.fogParams.color);
        scene->fogDensity   = p.fogParams.density;
        scene->fogMaximum   = p.fogParams.maximum;                

        assert( scene->camera != nullptr );
        assert( scene->viewPlane != nullptr );
        assert( !scene->materials.empty() );
        assert( scene->materials[0] != nullptr );
        assert( !scene->objects.empty() );
        assert( scene->objects[0] != nullptr );

        // No lights, or at least one valid light.
        assert( scene->lights.empty() || scene->lights[0] != nullptr );
        
        assert( scene->sceneSampler != nullptr );
        return scene;
    }


} // namespace glow
