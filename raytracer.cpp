#include "include/Raytracer/glFigures.hpp"
#include "include/Raytracer/glLights.hpp"
#include "include/Raytracer/glRaytracer.hpp"
#include "include/glBmp.hpp"
// #include <cstdint>

int main () {
    const uint32_t width = 1024;
    const uint32_t height = 1024;

    Material snow {{1, 1, 1}, 16};
    Material stone {{0.4, 0.4, 0.4}, 64, REFLECTIVE};
    Material orange {{0, 0.65, 1}, 64, TRANSPARENT, 1.5};
    Material grape {{1, 0, 1}, 16};
    Material mint {{0.26, 0.37, 0.32}, 64, REFLECTIVE};
    Material wood {{0.52, 0.37, 0.26}, 64, TRANSPARENT, 1.5};


    GlRaytracer rtx {width, height};

    rtx.glClearColor(0.5, 0.5, 0.5);
    rtx.glClear();

    rtx.setEnvMap(BmpFile("data/Raytracer/panoramic-beach.bmp"));

    AmbientLight amb_l = {0.1};
    DirectionalLight dir_l = {{-1, -1, -1}, 0.8};

    rtx.addLight(&amb_l);
    rtx.addLight(&dir_l);

    Plane p ({0, -10, 0}, {0,1,0}, grape);
    rtx.addObject(&p);

    rtx.glRender();

    rtx.glFinish();
}