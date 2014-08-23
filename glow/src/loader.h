// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599

#pragma once


#include <string>
#include <vector>


namespace glow {


    // Runs just like this is the command line for main.
    int CommandLineRun(int argc, char** argv);

    int Run(const std::vector<std::string>& args);


} // glow
