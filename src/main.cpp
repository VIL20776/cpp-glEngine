#include "../include/glRender3D.hpp"
#include "../include/glShaders.hpp"

#include <iostream>
#include <ostream>

int main() {
    GlRender3D render;

    render.glCreateWindow(960, 540);
    std::cout << "window created" << endl;
    render.glTexture(BmpFile ("../texture.bmp"));
    std::cout << "texture loaded" << endl;
    render.glShader(&flat);
    std::cout << "shader loaded" << endl;
    render.glLoadModel("../fox.obj");

    render.glFinish();
}