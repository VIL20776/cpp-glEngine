#ifndef GLBMPRENDER_H
#define GLBMPRENDER_H

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <vector>

#include "glBmp.hpp"

using namespace std;

class GlBmpRender {
    protected:
        // BMP components
        uint32_t width;
        uint32_t height;
        vector<Color> pixels {};

        BmpFile image {};

        // Color Pallete
        Color clear;
        Color point;
        Color fill;

        // Utilities
        ViewPort viewport;
        
        // Aux Functions
        bool evenOdd(uint32_t x, uint32_t y, float poly [][2], size_t num); 

    public:
        void glInit();

        void glCreateWindow(uint32_t width, uint32_t height);

        void glViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        void glClear();

        void glClearColor(float r, float g, float b);

        void glWPoint(uint32_t x, uint32_t y);

        void glVPoint(float x, float y);

        void glPointColor(float r, float g, float b);

        void glLine(float v0 [], float v1 []);

        void glDraw(float poligon [][2], size_t size);

        void glFill(float poly[][2], size_t size);

        void glFinish();
};

#endif
