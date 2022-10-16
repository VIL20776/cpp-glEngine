#ifndef GLRAYTRACER_H
#define GLRAYTRACER_H

#include "../glRender3D.hpp"
#include "glFigures.hpp"
#include "glLights.hpp"
#include "../glBmp.hpp"
#include "../glStructs.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

const int MAX_RECURSION_DEPTH = 3;

class GlRaytracer;
static GlRaytracer *glrtx = nullptr;
Intersect globalSceneIntersect (GlRaytracer *glrtx, std::vector<float> orig, std::vector<float> dir, Object *sceneObj);
class GlRaytracer: public GlRender3D
{
    protected:
    float fov = 60;
    float nearPlane = 0.1;

    vector<float> camPosition {0, 0, 0};
    vector<Model> models {};
    vector<Object*> scene {};
    vector<Light*> lights {};
    BmpFile envMap = {};
    
    public:
    GlRaytracer (uint32_t width, uint32_t height); 

    void addLight (Light *light);

    void addObject (Object *object);

    void setEnvMap (BmpFile envMap);

    Intersect scene_intersect (std::vector<float> orig, std::vector<float> dir, Object *sceneObj);

    std::vector<float> cast_ray (std::vector<float> orig, std::vector<float> dir, Object *sceneObj = nullptr, int recursion = 0);

    void glLoadModel(string filename,
        array<float, 3> translate, array<float, 3> rotate, array<float, 3> scale,
        Material material);

    void glRender ();
};

#endif