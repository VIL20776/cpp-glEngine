#ifndef GLFIGURES_H
#define GLFIGURES_H

#include <array>
#include <cmath>
#include <vector>

#include "../liblinalg.hpp"

const std::array<float, 3> WHITE {1, 1, 1};
const std::array<float, 3> BLACK {0, 0, 0};

class Object;

struct Intersect {
    bool null {true};
    float distance {};
    std::vector<float> point {};
    std::vector<float> normal {};
    Object *sceneObj; 
};

struct Material {
    std::array<float, 3> diffuse {WHITE};
};

class Object
{
    Material material;

    public:
    virtual Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir) = 0;

    Material getMaterial ()
    {
        return material;
    }
};

class Sphere : public Object {
    std::vector<float> center;
    float radius;
    Material material;

    public:
    Sphere (std::vector<float> center, float radius, Material material);

    Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir);
};
#endif