#include "../../include/Raytracer/glLights.hpp"
#include <algorithm>
#include <cmath>
#include <vector>

std::vector<float> reflectVector (std::vector<float> normal, std::vector<float> direction)
{
    float fReflect = 2 * dot(normal, direction);
    std::vector<float> vReflect = mult(fReflect, normal);
    vReflect = substract(vReflect, direction);
    vReflect = normalize(vReflect);
    return vReflect;
}

std::vector<float> refractVector (std::vector<float> normal, std::vector<float> direction, float index)
{
    float cosi = std::max((float) -1, std::min((float) 1, dot(direction, normal)));
    float etai = 1;
    float etat = index;

    if (cosi < 0)
    {
        cosi = -cosi;
    } else {
        float temp = etat;
        etat = etai;
        etai = temp;
        normal = negateV(normal);
    }

    float eta = etai / etat;
    float k = 1 - std::pow(eta, 2) * (1 - std::pow(cosi, 2));

    if (k < 0) {
        return {};
    }

    std::vector<float> R = add(mult(eta, direction), mult((eta * cosi - std::sqrt(k)), normal));
    return R;
}

double fresnel (std::vector<float> normal, std::vector<float> direction, float cr)
{
    float cosi = std::max((float) -1, std::min((float) 1, dot(direction, normal)));
    float etai = 1;
    float etat = cr;

    if (cosi > 0)
    {
        float temp = etai;
        etai = etat;
        etat = temp;
    }

    double sint = etai / etat * std::sqrt(std::max((double) 0, 1 - std::pow(cosi, 2)));

    if (sint >= 1) return 1;

    double cost = std::sqrt(std::max((double) 0, 1 - std::pow(sint, 2)));
    cosi = std::abs(cosi);
    
    double Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
    double Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));

    return (std::pow(Rs, 2) + std::pow(Rp, 2)) / 2;
}


// Directional
DirectionalLight::DirectionalLight (std::vector<float> direction, float intensity, std::vector<float> color)
{
    this->direction = normalize(direction);
    this->intensity = intensity;
    this->color = color;
    this->lightType = DIR_LIGHT;
}

std::vector<float> DirectionalLight::getDiffuseColor(Intersect intersect)
{
    std::vector<float> lightDir = mult(-1, this->direction);

    float intensity = dot(intersect.normal, lightDir) * this->intensity;
    intensity = (0 > intensity) ? 0: intensity;

    std::vector<float> diffuseColor = {{
        intensity * color.at(0),
        intensity * color.at(1),
        intensity * color.at(2)
    }};

    return diffuseColor;
}

std::vector<float> DirectionalLight::getSpecColor(Intersect intersect, std::vector<float> *camPosition)
{
    std::vector<float> light_dir = mult(-1, this->direction);
    std::vector<float> reflect = reflectVector(intersect.normal, light_dir);

    std::vector<float> viewDir = substract(*camPosition, intersect.point);
    viewDir = normalize(viewDir);

    float spec_intensity = this->intensity * std::pow(std::max((float) 0, dot(viewDir, reflect)), intersect.sceneObj->getMaterial().spec);
    std::vector<float> specColor = {{
        spec_intensity * this->color.at(0),
        spec_intensity * this->color.at(1),
        spec_intensity * this->color.at(2)
    }};
    
    return specColor;
}

float DirectionalLight::getShadowIntensity(Intersect intersect, Intersect (*scene_intersect)(std::vector<float>, std::vector<float>, Object *))
{
    std::vector<float> light_dir = mult(-1, this->direction);

    float shadow_intensity = 0;
    Intersect shadow_intersect = scene_intersect(intersect.point, light_dir, intersect.sceneObj);
    if (!shadow_intersect.null) shadow_intensity = 1;

    return shadow_intensity;
}

// Point
PointLight::PointLight (std::vector<float> point,
    float constant, float linear, float quad,
    std::vector<float> color)
{
    this->point = point;
    this->constant = constant;
    this->linear = linear;
    this->quad = quad;
    this->color = color;
    this->lightType = POINT_LIGHT;
}


std::vector<float> PointLight::getDiffuseColor(Intersect intersect)
{
    std::vector<float> lightDir = substract(this->point, intersect.point);
    lightDir = normalize(lightDir);

    float attenuation = 1.0;
    float intensity = dot(intersect.normal, lightDir) * attenuation;
    intensity = (0 > intensity) ? 0: intensity;

    std::vector<float> diffuseColor = {{
        intensity * color.at(0),
        intensity * color.at(1),
        intensity * color.at(2)
    }};

    return diffuseColor;
}

std::vector<float> PointLight::getSpecColor(Intersect intersect, std::vector<float> *camPosition)
{
    std::vector<float> lightDir = substract(this->point, intersect.point);
    lightDir = normalize(lightDir);
    std::vector<float> reflect = reflectVector(intersect.normal, lightDir);

    std::vector<float> viewDir = substract(*camPosition, intersect.point);
    viewDir = normalize(viewDir);

    float attenuation = 1;
    float spec_intensity = attenuation * std::pow(std::max((float) 0, dot(viewDir, reflect)), intersect.sceneObj->getMaterial().spec);
    std::vector<float> specColor = {{
        spec_intensity * this->color.at(0),
        spec_intensity * this->color.at(1),
        spec_intensity * this->color.at(2)
    }};

    return specColor;
}

float PointLight::getShadowIntensity(Intersect intersect, Intersect (*scene_intersect)(std::vector<float>, std::vector<float>, Object *))
{
    std::vector<float> light_dir = substract(this->point, intersect.point);
    light_dir = normalize(light_dir);

    float shadow_intensity = 0;
    Intersect shadow_intersect = scene_intersect(intersect.point, light_dir, intersect.sceneObj);
    if (!shadow_intersect.null) shadow_intensity = 1;

    return shadow_intensity;
}

// Ambient
AmbientLight::AmbientLight (float intensity, std::vector<float> color)
{
    this->intensity = intensity;
    this->color = color;
    this->lightType = AMBIENT_LIGHT;
}

std::vector<float> AmbientLight::getDiffuseColor (Intersect intersect)
{
    return mult(this->intensity,this->color);
}

std::vector<float> AmbientLight::getSpecColor(Intersect intersect, std::vector<float> *camPosition)
{
    return {0, 0, 0};
}

float AmbientLight::getShadowIntensity(Intersect intersect, Intersect (*scene_intersect)(std::vector<float>, std::vector<float>, Object *))
{
    return 0;
}

