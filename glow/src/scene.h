// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//
// This file contains the one and only scene object.
//

#pragma once

#include "color.h"

#include <memory>
#include <vector>


namespace glow {


    class Camera;
    class ViewPlane;
    class Material;
    class Object;
    struct SceneParams;
    class Scene;
    class Recorder;
    class Light;
    class Sampler;
    class SceneSampler;
    class BackgroundSampler;
    struct Ray;

    typedef std::unique_ptr<Camera> CameraPtr;
    typedef std::unique_ptr<ViewPlane> ViewPlanePtr;
    typedef std::unique_ptr<Material> MaterialPtr;
    typedef std::vector<MaterialPtr> MaterialSeq;
    typedef std::unique_ptr<Object> ObjectPtr;
    typedef std::vector<ObjectPtr> ObjectSeq;
    typedef std::unique_ptr<Scene> ScenePtr;
    typedef std::unique_ptr<Recorder> RecorderPtr;
    typedef std::unique_ptr<Light> LightPtr;
    typedef std::vector<LightPtr> LightSeq;
    typedef std::unique_ptr<Sampler> SamplerPtr;
    typedef std::unique_ptr<SceneSampler> SceneSamplerPtr;
    typedef std::unique_ptr<BackgroundSampler> BackgroundSamplerPtr;

    class Scene {
    public:
        // No copying or assigning this.  Only the factory can make a Scene.
        Scene& operator=(const Scene&) = delete;
        Scene(const Scene&) = delete;
        ~Scene();

        const CameraPtr& GetCamera() const { return camera; }        
        const ViewPlanePtr& GetViewPlane() const { return viewPlane; }
        const MaterialSeq& GetMaterials() const { return materials; }
        const ObjectSeq& GetObjects() const { return objects; }
        Color GetBackground(const Ray& r, int x, int y) const;
        const Color& GetAmbient() const { return ambient; }

        bool IsFoggy() const { return fogDensity > 0.0f; }
        float GetFogDensity() const { return fogDensity; }
        const Color& GetFogColor() const { return fogColor; }
        float GetFogMaximum() const { return fogMaximum; }
        
        const SceneSamplerPtr& GetSceneSampler() const { return sceneSampler; }

        const RecorderPtr& GetColorRecorder() const;
        void SetColorRecorder(RecorderPtr& rec);

        const RecorderPtr& GetDepthRecorder() const;
        void SetDepthRecorder(RecorderPtr& rec);

        const LightSeq& GetLights() const;
        void SetLights(LightSeq& ls);

        // Most samplers are based upon a linear PRNG that requires state.
        // Multithreading plays merry havoc with the state information.
        // Instead of returning a single sampler, the scene returns a clone to
        // be used within the camera's render cycle.
        SamplerPtr CloneSampler() const;
        int GetNumSamples() const { return numSamples; }

    private:
        CameraPtr       camera;
        ViewPlanePtr    viewPlane;
        MaterialSeq     materials;
        ObjectSeq       objects;
        Color           background;
        Color           ambient;
        RecorderPtr     colorRecorder;
        RecorderPtr     depthRecorder;
        LightSeq        lights;

        float           fogDensity;
        Color           fogColor;
        float           fogMaximum;

        int             numSamples;
        SamplerPtr      sampler;
        SceneSamplerPtr sceneSampler;

        BackgroundSamplerPtr backgroundSampler;

        Scene();
        friend ScenePtr MakeScene(const SceneParams& sp);
    };


} // namespace glow
