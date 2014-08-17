// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//
// This file defines the recorder for recording to PNG files.

#pragma once


#include "recorder.h"

#include <png.h>
#include <zlib.h>
#include <stdio.h>


namespace glow {


    class RecorderPng : public Recorder {
    public:
        RecorderPng(const char* out, int w, int h, int depth, const char* comment);
        ~RecorderPng();

    private:
        std::string out_;
        int w_;
        int h_;
        int depth_;
        int pixSize_;
        std::string comment_;

        FILE* fp_;
        png_structp png_ptr_;
        png_infop png_info_ptr_;

        typedef std::vector< png_byte > ArrayBytes;
        ArrayBytes array_bytes_;

        void RecordColorImpl(const ColorFilm& film) override;
        void RecordDepthImpl(const DepthFilm& film) override;

        void Init();
        void Flush();
        void Finish();
        void SetPixel(int x, int y, float red, float green, float blue);
    };



} // namespace glow
