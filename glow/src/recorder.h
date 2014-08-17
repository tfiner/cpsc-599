// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once


#include "color.h"
#include <string>
#include <vector>


namespace glow {


    using ColorFilm = std::vector<Color>;
    using DepthFilm = std::vector<float>;


    class Recorder {
    public:
        Recorder() {}
        virtual ~Recorder() {}

        void RecordColor(const ColorFilm& film) { 
            RecordColorImpl(film); 
        }
        void RecordDepth(const DepthFilm& film) { 
            RecordDepthImpl(film); 
        }

    private:
        virtual void RecordColorImpl(const ColorFilm&) {};
        virtual void RecordDepthImpl(const DepthFilm&) {};
    };


} // namespace glow
