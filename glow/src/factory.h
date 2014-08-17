// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//
// This file contains the factory for all of the raytracing objects.

#pragma once

#include <memory>


namespace glow {


    class Scene;
    struct SceneParams;
    typedef std::unique_ptr<Scene> ScenePtr;
    ScenePtr MakeScene( const SceneParams& params );


} // namespace glow
