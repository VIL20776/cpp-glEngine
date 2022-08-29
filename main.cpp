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
        {width/2, height/2, 0},
        {0, 0, 0},
        {2, 2, 2});

    render.glFinish();
}