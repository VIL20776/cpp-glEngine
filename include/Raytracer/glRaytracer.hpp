#ifndef GLRAYTRACER_H
#define GLRAYTRACER_H

#include "../glBmpRender.hpp"
#include "glFigures.hpp"
#include "glLights.hpp"
#include "../glStructs.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

class GlRaytracer: public GlBmpRender
{
    protected:
    float fov = 60;
    float nearPlane = 0.1;

    vector<float> camPosition {0, 0, 0};
    vector<Object*> scene {};
    vector<Light> lights {};
    
    public:
    GlRaytracer (uint32_t width, uint32_t height); 

    void addLight (Light light);

    void addObject (Object *object);

    Intersect scene_intersect (std::vector<float> orig, std::vector<float> dir, Object *sceneObj);

    std::vector<float> cast_ray (std::vector<float> orig, std::vector<float> dir);

    void glRender ();

};

#endif