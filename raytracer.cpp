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

    Material glass {{0.9, 0.9, 0.9}, 64, TRANSPARENT, 1.5};

    Material obama {
        .spec = 16,
        .texture = BmpFile ("data/mandala.bmp")
    };

    GlRaytracer rtx {width, height};

    rtx.glClearColor(0.5, 0.5, 0.5);
    rtx.glClear();

    rtx.setEnvMap(BmpFile("data/Raytracer/panoramic-beach.bmp"));

    AmbientLight amb_l = {0.2};
    DirectionalLight dir_l = {{-1, -1, -1}, 0.9};

    rtx.addLight(&amb_l);
    rtx.addLight(&dir_l);

    Triangle t0 ({2, 1, -10}, {-2, 1, -10}, {0, 5, -10}, grape);
    Triangle t1 ({0, -3, -10}, {4, -3, -10}, {2, 1, -10}, stone);
    Triangle t2 ({-4, -3, -10}, {0, -3, -10}, {-2, 1, -10}, orange);

    // Sphere s ({0, 0, -5}, 0.5, obama);
    // rtx.addObject(&s);

    rtx.addObject(&t0);
    rtx.addObject(&t1);
    rtx.addObject(&t2);


    rtx.glRender();

    rtx.glFinish();
}