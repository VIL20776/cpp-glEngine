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

    Sphere o1 ({-1, -2, -5}, 0.8, snow);
    Sphere r1 ({1, -2, -5}, 0.8, stone);
    Sphere t1 ({-1, -0.3, -5}, 0.8, orange);
    Sphere o2 ({1, -0.3, -5}, 0.8, grape);
    Sphere r2 ({-1, 1.4, -5}, 0.8, mint);
    Sphere t2 ({1, 1.4, -5}, 0.8, wood);

    rtx.addObject(&o1);
    rtx.addObject(&r1);
    rtx.addObject(&t1);
    rtx.addObject(&o2);
    rtx.addObject(&r2);
    rtx.addObject(&t2);

    rtx.glRender();

    rtx.glFinish();
}