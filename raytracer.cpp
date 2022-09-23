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
    Material nose {{0, 0.65, 1}, 64, TRANSPARENT, 1.5};

    GlRaytracer rtx {width, height};
    rtx.glClearColor(0.5, 0.5, 0.5);
    rtx.glClear();

    rtx.setEnvMap(BmpFile("data/Raytracer/panoramic-beach.bmp"));

    AmbientLight amb_l = {0.1};
    DirectionalLight dir_l = {{-1, -1, -1}, 0.8};

    rtx.addLight(&amb_l);
    rtx.addLight(&dir_l);

    Sphere s1 ({0, -3.3, -10}, 2, snow);
    Sphere d1 ({0, -1.6, -5}, 0.2, stone);
    Sphere n ({0, 1, -5}, 0.1, nose);

    rtx.addObject(&s1);
    rtx.addObject(&d1);
    rtx.addObject(&n);

    rtx.glRender();

    rtx.glFinish();
}