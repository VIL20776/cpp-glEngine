#include "../../include/liblinalg.hpp"
#include "../../include/glStructs.hpp"
#include <array>
#include <vector>

const float DIR_LIGHT = 0;
const float POINT_LIGHT = 1;
const float AMBIENT_LIGHT = 2;

class Light
{
    protected:
    unsigned char lightType;
    std::vector<float> direction;
    float intensity;
    std::vector<float> color;

    public:
    unsigned char getType ()
    {
        return this->lightType;
    }

    std::vector<float> getDirection ()
    {
        return this->direction;
    }

    float getIntensity ()
    {
        return this->intensity;
    }

    std::vector<float> getColor ()
    {
        return this->color;
    }
};

class DirectionalLight: public Light
{
    public:
    // Directional
    DirectionalLight (std::vector<float> direction = {0, -1, 0}, float intensity = 1, std::vector<float> color = {1, 1, 1})
    {
        this->direction = normalize(direction);
        this->intensity = intensity;
        this->color = color;
        this->lightType = DIR_LIGHT;
    }
};

class AmbientLight: public Light
{
    public:
    // Ambient
    AmbientLight (float intensity = 0.1, std::vector<float> color = {1, 1, 1})
    {
        this->intensity = intensity;
        this->color = color;
        this->lightType = AMBIENT_LIGHT;
    }


};
