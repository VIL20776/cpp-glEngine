#ifndef GLLIGHTS_H
#define GLLIGHTS_H
#include "../../include/liblinalg.hpp"
#include "../../include/glStructs.hpp"
#include "glFigures.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <sys/types.h>
#include <vector>

const u_char DIR_LIGHT = 0;
const u_char POINT_LIGHT = 1;
const u_char AMBIENT_LIGHT = 2;

std::vector<float> reflectVector (std::vector<float> normal, std::vector<float> direction);

std::vector<float> refractVector (std::vector<float> normal, std::vector<float> direction, float index);

double fresnel (std::vector<float> normal, std::vector<float> direction, float cr);

class Light
{
    protected:
    unsigned char lightType;

    public:
    virtual std::vector<float> getDiffuseColor (Intersect intersect) = 0;

    virtual std::vector<float> getSpecColor (Intersect intersect, std::vector<float> *camPosition) = 0;

    virtual float getShadowIntensity (
        Intersect intersect,
        Intersect (*scene_intersect) (std::vector<float> orig, std::vector<float> dir, Object *sceneObj)
    ) = 0;
};

class DirectionalLight: public Light
{
    private:
    std::vector<float> direction;
    float intensity;
    std::vector<float> color;

    public:
    DirectionalLight (
        std::vector<float> direction = {0, -1, 0},
        float intensity = 1,
        std::vector<float> color = {1, 1, 1}
    );

    std::vector<float> getDiffuseColor (Intersect intersect);

    std::vector<float> getSpecColor (Intersect intersect, std::vector<float> *camPosition);

    float getShadowIntensity (Intersect intersect, Intersect (*scene_intersect) (std::vector<float> orig, std::vector<float> dir, Object *sceneObj));
};

class PointLight: public Light 
{   
    private:
    std::vector<float> point;
    float constant;
    float linear;
    float quad;
    std::vector<float> color;

    public:
    PointLight(
        std::vector<float> point,
        float constant = 1,
        float linear = 0.1,
        float quad = 0.05,
        std::vector<float> color = {1,1,1}
    );

    std::vector<float> getDiffuseColor (Intersect intersect);

    std::vector<float> getSpecColor (Intersect intersect, std::vector<float> *camPosition);

    float getShadowIntensity (Intersect intersect, Intersect (*scene_intersect) (std::vector<float> orig, std::vector<float> dir, Object *sceneObj));
};

class AmbientLight: public Light
{
    private:
    float intensity;
    std::vector<float> color;

    public:
    AmbientLight (
        float intensity = 0.1,
        std::vector<float> color = {1,1,1}
    );

    std::vector<float> getDiffuseColor (Intersect intersect);

    std::vector<float> getSpecColor (Intersect intersect, std::vector<float> *camPosition);

    float getShadowIntensity (Intersect intersect, Intersect (*scene_intersect) (std::vector<float> orig, std::vector<float> dir, Object *sceneObj));
};

#endif