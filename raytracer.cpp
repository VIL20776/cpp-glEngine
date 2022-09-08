#include "include/Raytracer/glRaytracer.hpp"
// #include <cstdint>

int main () {
    const uint32_t width = 1024;
    const uint32_t height = 1024;

    Material brick {{0.8, 0.3, 0.3}};
    Material stone {{0.4, 0.4, 0.4}};
    Material grass {{0.3, 1, 0.3}};

    GlRaytracer rtx {width, height};

    rtx.addLight(AmbientLight ());
    rtx.addLight(DirectionalLight ({0, 0, -1}));

    Sphere c1 ({0, -2, -10}, 2, brick);
    Sphere c2 ({0, -0, -10}, 1.5, stone);
    Sphere c3 ({0, 2, -10}, 1, grass);

    rtx.addObject(&c1);
    rtx.addObject(&c2);
    rtx.addObject(&c3);

    rtx.glRender();

    rtx.glFinish();
}