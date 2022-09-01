#include "include/glBmp.hpp"
#include "include/glRender3D.hpp"
#include "include/glShaders.hpp"

#include <iostream>
#include <ostream>

int main() {
    GlRender3D render;

    const float width = 960;
    const float height = 540;

    render.glInit();
    render.glCreateWindow(width, height);
    render.glClear();
    std::cout << "window created" << endl;
    render.glTexture(BmpFile ("texture.bmp"));
    std::cout << "texture loaded" << endl;
    render.glShader(&flat);
    std::cout << "shader loaded" << endl;
    render.glLoadModel(
        "fox.obj",
        {4, -2, -9},
        {0, 35, 0},
        {0.05, 0.05, 0.05});

    render.glFinish();

    return 0;
}