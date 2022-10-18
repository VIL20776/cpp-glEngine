#include "include/Raytracer/glFigures.hpp"
#include "include/Raytracer/glLights.hpp"
#include "include/Raytracer/glRaytracer.hpp"
#include "include/glBmp.hpp"
#include <vector>
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

    Material grass {
        .texture = BmpFile ("data/Raytracer/grass-lawn-texture.bmp")
    };

    GlRaytracer rtx {width, height};

    rtx.setEnvMap(BmpFile("data/Raytracer/panoramic-beach.bmp"));

    AmbientLight amb_l = {0.2};
    DirectionalLight dir_l = {{-1, -1, -1}, 0.9};

    rtx.addLight(&amb_l);
    rtx.addLight(&dir_l);

    Plane p1 ({0, -10, 0}, {0,1,0}, snow);

    Triangle t0 ({2, 1, -10}, {-2, 1, -10}, {0, 5, -10}, grape);
    Triangle t1 ({0, -3, -10}, {4, -3, -10}, {2, 1, -10}, stone);
    Triangle t2 ({-4, -3, -10}, {0, -3, -10}, {-2, 1, -10}, orange);

    Sphere s0 ({0, -2, -5}, 0.2, grass);

    MineCube c1 ({1, 1, 1}, {-3, 3, -10}, stone);
    MineCube c2 ({1, 1, 1}, {3, 3, -10}, mint);

    std::vector<Sphere> spheres {};
    for (float i = 0; i < 5; i++) {
        spheres.push_back(Sphere ({(i - 2.5f), -1, -8}, 0.2, mint));
        spheres.push_back(Sphere ({(i - 2.5f), 1, -8}, 0.2, mint));
    }

    for (auto &sph : spheres) {
        rtx.addObject(&sph);
    }

    rtx.addAllObjects({&p1, &t0, &t1, &t2, &s0, &c1, &c2});

    rtx.glRender();

    rtx.glFinish();
}