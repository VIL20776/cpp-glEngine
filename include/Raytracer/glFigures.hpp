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
    std::vector<int> texCoords {};
};

struct Material {
    std::array<float, 3> diffuse {WHITE};
    float spec {1};
    char type {OPAQUE};
    float ir;
    BmpFile texture {};
};

class Object
{
    protected:
    Material material;

    public:
    virtual Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir) = 0;

    virtual Material getMaterial ();
};

class Sphere : public Object {
    std::vector<float> center;
    float radius;

    public:
    Sphere (std::vector<float> center, float radius, Material material);

    Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir);
};

class Plane: public Object {
    private:
    std::vector<float> position;
    std::vector<float> normal;

    public:
    Plane(std::vector<float> position, std::vector<float> normal, Material material);

    Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir);
};

// Axis Aligned Bounding Box
class MineCube: public Object {
    private:
    std::vector<float> size;
    std::vector<float> position;
    std::vector<Plane> planes {};

    std::vector<float> boundMin {0, 0, 0};
    std::vector<float> boundMax {0, 0, 0};

    public:
    MineCube (std::vector<float> size, std::vector<float> position, Material material);

    Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir);
};

class Triangle: public Object {
    private:
    std::vector<float> A, B, C;
    Plane plane;

    public:
    Triangle (std::vector<float> A, std::vector<float> B, std::vector<float> C, Material material);

    Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir);
};

#endif