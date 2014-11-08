// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once

#include "log_msg.h"
#include <tuple>

namespace glow {


    class ViewPlane {
    public:
        ViewPlane(int x0, int y0, int x1, int y1, float ps) : 
            x0_(x0), y0_(y0), 
            x1_(x1), y1_(y1), 
            pixelSize_(ps) {}            

        // Returns the horizontal span [X0, X1)
        std::tuple<int,int> GetHSpanPixels() const {
            auto x0 = static_cast<int>(x0_ / pixelSize_);
            auto x1 = static_cast<int>((x1_ / pixelSize_) + 0.5f);
            return std::make_tuple(x0, x1);
        }

        // Returns the vertical span [Y0, Y1)
        std::tuple<int,int> GetVSpanPixels() const {
            auto y0 = static_cast<int>(y0_ / pixelSize_);
            auto y1 = static_cast<int>((y1_ / pixelSize_) + 0.5f);
            return std::make_tuple(y0, y1);
        }

        int GetWidthPixels() const { 
            auto const span = GetHSpanPixels();        
            return std::get<1>(span) - std::get<0>(span);
        }

        int GetHeightPixels() const { 
            auto const span = GetVSpanPixels();        
            return std::get<1>(span) - std::get<0>(span);
        }

        void SetImgAreaPixels(int x0, int y0, int x1, int y1) {
            x0_ = x0 * pixelSize_;
            y0_ = y0 * pixelSize_;
            x1_ = x1 * pixelSize_;
            y1_ = y1 * pixelSize_;

            LOG_MSG(1, "Viewplane  " 
                << "pixel size: " << pixelSize_ << " "
                << "x0: " << x0_ << " "
                << "y0: " << y0_ << " "
                << "x1: " << x1_ << " "
                << "y1: " << y1_ );
        }

        float GetPixelSize() const { return pixelSize_; }

    private:
        // Viewplane in world units
        float x0_, y0_;
        float x1_, y1_;

        float pixelSize_;        
    };


} // namespace glow