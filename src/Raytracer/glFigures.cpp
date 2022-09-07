#include <array>
#include <cmath>
#include <vector>

#include "../../include/liblinalg.hpp"

const std::array<float, 3> WHITE {1, 1, 1};
const std::array<float, 3> BLACK {0, 0, 0};

class Sphere;

struct Intersect {
    bool null {true};
    float distance {};
    std::vector<float> point {};
    std::vector<float> normal {};
    Sphere *sceneObj {}; 
};

struct Material {
    std::array<float, 3> diffuse {WHITE};
};

class Object
{
    Material material;

    public:
    Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir);

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
    Sphere (std::vector<float> center, float radius, Material material)
    {
        this->center = center;
        this->radius = radius;
        this->material = material;
    }

    Intersect ray_intersect (std::vector<float> orig, std::vector<float> dir)
    {
        std::vector<float> L = substract(center, orig);
        float tca = dot(L, dir);
        float d = std::pow((std::pow(magnitude(L), 2) - std::pow(tca,2)), 0.5);

        if (d > radius){
            return Intersect {};
        }

        float thc = std::pow((std::pow(radius, 2) - std::pow(d, 2)), 0.5);

        float t0 = tca - thc;
        float t1 = tca + thc;

        if (t0 < 0) {
            t0 = t1;
        }
        if (t0 < 0) {
            return Intersect {};
        }

        std::vector<float> P = add(orig, mult(t0, dir));
        std::vector<float> normal = substract(P, center);
        normal = normalize(normal);

        return Intersect {false, t0, P, normal, this};

    }
};