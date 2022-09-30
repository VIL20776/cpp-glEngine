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

    AmbientLight amb_l = {0.2};
    // DirectionalLight dir_l = {{-0.1, 0, -1}, 0.8};
    PointLight point_l = {{0, 0, -5}};

    rtx.addLight(&amb_l);
    // rtx.addLight(&dir_l);
    rtx.addLight(&point_l);

    Plane p1 ({0, -10, 0}, {0,1,0}, snow);
    Plane p2 ({0, 10, 0}, {0,-1,0}, snow);
    Plane p3 ({-10, 0, 0}, {1,0,0}, snow);
    Plane p4 ({10, 0, 0}, {-1,0,0}, snow);
    Plane p5 ({0, 0, -20}, {0,0,1}, snow);

    rtx.addObject(&p1);
    rtx.addObject(&p2);
    rtx.addObject(&p3);
    rtx.addObject(&p4);
    rtx.addObject(&p5);

    MineCube c1 ({2, 2, 2}, {-3, -2, -10}, stone);
    MineCube c2 ({2, 2, 2}, {3, -2, -10}, mint);

    rtx.addObject(&c1);
    rtx.addObject(&c2);


    rtx.glRender();

    rtx.glFinish();
}