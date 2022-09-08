#include "../../include/Raytracer/glLights.hpp"

// Directional
DirectionalLight::DirectionalLight (std::vector<float> direction, float intensity, std::vector<float> color)
{
    this->direction = normalize(direction);
    this->intensity = intensity;
    this->color = color;
    this->lightType = DIR_LIGHT;
}

// Ambient
AmbientLight::AmbientLight (float intensity, std::vector<float> color)
{
    this->intensity = intensity;
    this->color = color;
    this->lightType = AMBIENT_LIGHT;
}



