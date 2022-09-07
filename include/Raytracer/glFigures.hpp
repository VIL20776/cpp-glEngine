#include <array>
#include <cmath>
#include <vector>

#include "../liblinalg.hpp"

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