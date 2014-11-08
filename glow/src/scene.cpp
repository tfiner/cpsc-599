// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file contains the one and only scene object.
//

#include "scene.h"
#include "view_plane.h"
#include "material.h"
#include "object.h"
#include "camera.h"
#include "recorder.h"
#include "light.h"
#include "sampler.h"
#include "scene_sampler.h"
#include "background_sampler.h"
#include "log_msg.h"

#include <assert.h>


using namespace glow;


Scene::Scene() :backgroundSampler(new NullBackground) {}
Scene::~Scene() {}


Color Scene::GetBackground(const Ray& r, int x, int y) const { 
    Color c(background);
    backgroundSampler->GetColor(*this, r, c, x, y);
    return c; 
}


const RecorderPtr& Scene::GetColorRecorder() const { return colorRecorder; }


void Scene::SetColorRecorder(RecorderPtr rec) { 
    assert( rec != nullptr );
    colorRecorder.reset(rec.release()); 
    assert( rec == nullptr );
}


const RecorderPtr& Scene::GetDepthRecorder() const { return depthRecorder; }


void Scene::SetDepthRecorder(RecorderPtr rec) { 
    swap(depthRecorder, rec);
}


const LightSeq& Scene::GetLights() const { 
    return lights; 
}


void Scene::SetLights(LightSeq ls) { 
    swap(lights, ls); 
}


SamplerPtr Scene::CloneSampler() const {
    return sampler->Clone();
}
