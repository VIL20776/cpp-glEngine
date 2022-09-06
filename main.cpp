#include "include/glBmp.hpp"
#include "include/glRender3D.hpp"
#include "include/glShaders.hpp"

int main() {
    GlRender3D render;

    const float width = 500;
    const float height = 500;

    // Setup
    render.glInit();
    render.glCreateWindow(width, height);
    render.glBackground(BmpFile("data/mandala.bmp"));
    // render.glClear();

    // Fox
    render.glTexture(BmpFile ("data/fox_texture.bmp"));
    render.glShader(&fried);
    render.glLoadModel(
        "data/fox.obj",
        {-8, 0, -20},
        {0, 45, 0},
        {0.05, 0.05, 0.05});

    // Don
    render.glTexture(BmpFile ("data/Don_Flip_Packed0_Diffuse.bmp"));
    render.glNormalMap(BmpFile ("data/Don_Flip_Packed0_Normal.bmp"));
    render.glShader(&normalMap);
    render.glLoadModel(
        "data/don-flip.obj",
        {1, 0, -7},
        {0, -45, 0},
        {0.5, 0.5, 0.5});
    
    // Obama ball
    render.glTexture(BmpFile ("data/obama.bmp"));
    render.glShader(&light);
    render.glLoadModel(
        "data/obama-ball.obj",
        {0, 0, -1},
        {0, 0, 0},
        {50, 50, 50});

    // Mario
    render.glTexture(BmpFile ("data/marioD.bmp"));
    render.glShader(&noise);
    render.glLoadModel(
        "data/mario-sculpture.obj",
        {-2.5, -2.5, -5},
        {0, 135, 0},
        {0.01, 0.01, 0.01});

    // Dona
    render.glTexture(BmpFile ("data/Annaleiva.bmp"));
    render.glShader(&gray);
    render.glLoadModel(
        "data/dona-teodora.obj",
        {0.5, -0.5, -1},
        {0, -135, 0},
        {0.1, 0.1, 0.1});

    render.glFinish();

    return 0;
}