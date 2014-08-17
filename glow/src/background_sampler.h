// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//
// This file contains the sampler for the background.
//

#pragma once 


namespace glow {


    struct Color;
    struct Ray;
    class Scene;


    class BackgroundSampler {
    public:
        void GetColor(const Scene& s, const Ray& r, Color& c, int x, int y) const {
            GetColorImpl(s,r,c,x,y);
        }
    private:
        virtual void GetColorImpl(const Scene& s, const Ray&, Color& c, int x, int y) const = 0;
    };


    class NullBackground : public BackgroundSampler {
    private:
        virtual void GetColorImpl(const Scene&, const Ray&, Color&, int, int) const {}
    };


} // glow
