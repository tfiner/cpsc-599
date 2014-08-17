// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//

#pragma once

namespace glow {


    class ViewPlane {
    public:
        ViewPlane(int w, int h, float ps) : width(w), height(h), pixelSize(ps) {}

        int GetWidthPixels() const { return static_cast<int>((width / pixelSize) + 0.5f); }
        int GetHeightPixels() const { return static_cast<int>((height / pixelSize) + 0.5f); }
        int GetWidthWorldUnits() const { return width; }
        int GetHeightWorldUnits() const { return height; }
        float GetPixelSize() const { return pixelSize; }

    private:
        // World units
        int width;
        int height;
        float pixelSize;        
    };


} // namespace glow