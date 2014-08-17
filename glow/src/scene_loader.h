// Tim Finer 
// tfiner@csu.fullerton.edu
// 
// This file contains the interface for the scene loader.

#pragma once 


#include <string>


namespace glow {


    struct SceneParams;


    // Loads the scene or throws an exception.
    SceneParams LoadScene(std::string& scene);
    

}

