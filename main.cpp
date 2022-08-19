#include "glBmp.cpp"
#include <cstdint>

int main() {
    GlBmp image;

    image.glInit();
    image.glCreateWindow(500, 500);
    // image.glClearColor(0.5, 0.5, 0.0);
    image.glClear();
    // image.glViewPort(32, 32, 100, 100);
    // image.glVPoint(0.5, 0.5);

    // image.glLine(v0, v1);

    float pol1 [10][2] = {
        {165, 380}, {185, 360}, {180, 330}, {207, 345}, {233, 330},
        {230, 360}, {250, 380}, {220, 385}, {205, 410}, {193, 383}
        };

    image.glFill(pol1, 10);
    image.glDraw(pol1,10);

    image.glFinish();
}