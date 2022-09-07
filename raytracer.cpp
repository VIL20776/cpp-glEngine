#include "src/Raytracer/glRaytracer.cpp"
// #include <cstdint>

int main () {
    const uint32_t width = 1024;
    const uint32_t height = 1024;

    Material brick {{0.8, 0.3, 0.3}};
    Material stone {{0.8, 0.3, 0.3}};
    Material grass {{0.8, 0.3, 0.3}};

    GlRaytracer rtx {width, height};

    rtx.addLight(AmbientLight ());
    rtx.addLight(DirectionalLight ({-1, -1, -1}));

    rtx.addObject(Sphere ({0, 0, -10}, 2, brick));
    rtx.addObject(Sphere ({-4, -2, -15}, 1.5, stone));
    rtx.addObject(Sphere ({2, 2, -8}, 0.2, grass));

    rtx.glRender();

    rtx.glFinish();
}