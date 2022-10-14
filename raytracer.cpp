#include "include/Raytracer/glFigures.hpp"
#include "include/Raytracer/glLights.hpp"
#include "include/Raytracer/glRaytracer.hpp"
#include "include/glBmp.hpp"
// #include <cstdint>

int main () {
    const uint32_t width = 256;
    const uint32_t height = 256;

    Material snow {{1, 1, 1}, 16};
    Material stone {{0.4, 0.4, 0.4}, 64, REFLECTIVE};
    Material orange {{0, 0.65, 1}, 64, TRANSPARENT, 1.5};
    Material grape {{1, 0, 1}, 16};
    Material mint {{0.26, 0.37, 0.32}, 64, REFLECTIVE};
    Material wood {{0.52, 0.37, 0.26}, 64, TRANSPARENT, 1.5};

    Material glass {{0.9, 0.9, 0.9}, 64, TRANSPARENT, 1.5};

    GlRaytracer rtx {width, height};

    rtx.setEnvMap(BmpFile("data/Raytracer/panoramic-beach.bmp"));

    AmbientLight amb_l = {0.2};
    DirectionalLight dir_l = {{-1, -1, -1}, 0.9};

    rtx.addLight(&amb_l);
    rtx.addLight(&dir_l);

    rtx.glLoadModel("data/fox.obj",
        {0, 0, -20},
        {0, 45, 0},
        {0.05, 0.05, 0.05},
        grape);

    rtx.glRender();

    rtx.glFinish();
}