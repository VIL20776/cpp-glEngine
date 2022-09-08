#include "include/Raytracer/glFigures.hpp"
#include "include/Raytracer/glRaytracer.hpp"
// #include <cstdint>

int main () {
    const uint32_t width = 1024;
    const uint32_t height = 1024;

    Material snow {{1, 1, 1}};
    Material dots {{0, 0, 0}};
    Material nose {{0, 1, 1}};

    GlRaytracer rtx {width, height};
    rtx.glClearColor(0.5, 0.5, 0.5);
    rtx.glClear();

    rtx.addLight(AmbientLight ());
    rtx.addLight(DirectionalLight ({0, 0, -1}));

    Sphere s1 ({0, -3.3, -10}, 2, snow);
    Sphere s2 ({0, -0.3, -10}, 1.5, snow);
    Sphere s3 ({0, 2, -10}, 1, snow);

    Sphere d1 ({0, -1.6, -5}, 0.2, dots);
    Sphere d2 ({0, -0.7, -5}, 0.2, dots);
    Sphere d3 ({0, 0.3, -5}, 0.2, dots);

    Sphere d4 ({0.1, 0.75, -5}, 0.05, dots);
    Sphere d5 ({-0.1, 0.75, -5}, 0.05, dots);
    Sphere d6 ({-0.25, 0.8, -5}, 0.05, dots);
    Sphere d7 ({0.25, 0.8, -5}, 0.05, dots);

    Sphere n ({0, 1, -5}, 0.1, nose);

    // Sphere s4 ({-0.2, 1.25, -5}, 0.1, dots);
    // Sphere s5 ({0.2, 1.25, -5}, 0.1, dots);

    Sphere d8 ({-0.2, 1.25, -5}, 0.05, dots);
    Sphere d9 ({0.2, 1.25, -5}, 0.05, dots);

    rtx.addObject(&s1);
    rtx.addObject(&s2);
    rtx.addObject(&s3);

    rtx.addObject(&d1);
    rtx.addObject(&d2);
    rtx.addObject(&d3);
 
    rtx.addObject(&d4);
    rtx.addObject(&d5);
    rtx.addObject(&d6);
    rtx.addObject(&d7);

    rtx.addObject(&n);

    rtx.addObject(&d8);
    rtx.addObject(&d9);

    rtx.glRender();

    rtx.glFinish();
}