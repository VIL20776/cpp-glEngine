#ifndef GLFIGURES_H
#define GLFIGURES_H

#include <array>
#include <cmath>
#include <vector>

#include "../liblinalg.hpp"
#include "../glBmp.hpp"

const std::array<float, 3> WHITE {1, 1, 1};
const std::array<float, 3> BLACK {0, 0, 0};

const char OPAQUE = 0;
const char REFLECTIVE = 1;
const char TRANSPARENT = 2;

class Object;

struct Intersect {
    bool null {true};
    float distance {};
    std::vector<float> point {};
    std::vector<float> normal {};
    Object *sceneObj;
    // std::vector<float> texCoords {};
};

struct Material {
    std::array<float, 3> diffuse {WHITE};
    float spec {1};
    char type {OPAQUE};
    float ir;
    BmpFile texture;
};

class Object
{
    Material material;

    public:
    virtual Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir) = 0;

    virtual Material getMaterial () = 0;
};

class Sphere : public Object {
    std::vector<float> center;
    float radius;
    Material material;

    public:
    Sphere (std::vector<float> center, float radius, Material material);

    Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir);

    Material getMaterial ();
};
#endif