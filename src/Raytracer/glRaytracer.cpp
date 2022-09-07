#include "../../include/glBmpRender.hpp"
#include "glFigures.cpp"
#include "glLights.cpp"
#include "../../include/glStructs.hpp"
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
    vector<Sphere> scene {};
    vector<Light> lights {};
    
    public:
    GlRaytracer (uint32_t width, uint32_t height)
    {
        glViewPort(0, 0, width, height);
        glClear();
    }

    void addLight (Light light)
    {
        lights.push_back(light);
    }

    void addObject (Sphere object)
    {
        scene.push_back(object);
    }

    Intersect scene_intersect (std::vector<float> orig, std::vector<float> dir, Sphere *sceneObj)
    {
        float depht = INFINITY;
        Intersect intersect {};

        for (auto& object : scene) {
            Intersect hit = object.ray_intersect(orig, dir);
            if (!hit.null) {
                if (sceneObj != hit.sceneObj) {
                    if (hit.distance < depht) {
                        intersect = hit;
                        depht = hit.distance;
                    }
                }
            }
        }
        return intersect;
    }

    Color cast_ray (std::vector<float> orig, std::vector<float> dir)
    {
        Intersect intersect = scene_intersect(orig, dir, nullptr);

        if (intersect.null) {
            return {0, 0, 0};
        }

        Material material = intersect.sceneObj->getMaterial();

        std::vector<float> finalColor = {0, 0, 0};
        std::vector<float> objectColor = {
            material.diffuse[0],
            material.diffuse[1],
            material.diffuse[2]};
        
        std::vector<float> dirLightColor = {0, 0, 0};
        std::vector<float> ambLightColor = {0, 0, 0};

        for (auto &light : lights) {
            switch (light.getType()) {
                case 0: {
                    std::vector<float> diffuseColor = {0, 0, 0};

                    std::vector<float> light_dir = negateV(light.getDirection());
                    float intensity = dot(intersect.normal, light_dir);
                    intensity = (0 < intensity) ? intensity : 0;

                    diffuseColor = {
                        intensity * light.getColor()[0] * light.getIntensity(),
                        intensity * light.getColor()[1] * light.getIntensity(),
                        intensity * light.getColor()[2] * light.getIntensity(),
                    };

                    float shadow_intensity = 0;
                    Intersect shadow_intersect = scene_intersect(intersect.point, light.getDirection(), intersect.sceneObj);
                    if (shadow_intersect.null) {
                        shadow_intensity = 1;
                    }
                    dirLightColor = add(dirLightColor, mult((1 - shadow_intensity), diffuseColor));
                    break;
                }
                case 2: {
                    ambLightColor = mult(light.getIntensity(), light.getColor());
                    break;
                }
            }
        }

        finalColor = add(dirLightColor, ambLightColor);
        finalColor = mult(finalColor, objectColor);

        float r = (1 > finalColor[0]) ? finalColor[0] : 1;
        float g = (1 > finalColor[1]) ? finalColor[1] : 1;
        float b = (1 > finalColor[2]) ? finalColor[2] : 1;

        return {(u_char) (r * 255), (u_char) (g * 255), (u_char) (b * 255)};
    }

    void glRender ()
    {
        for (size_t y = viewport.y; y < (viewport.y + viewport.height + 1); y++) {
            for (size_t x = viewport.x; x < (viewport.x + viewport.width + 1); x++) {
                
                float Px = (((x + 0.5 - viewport.x) / viewport.width) * 2) - 1;
                float Py = (((y + 0.5 - viewport.y) / viewport.height) * 2) - 1;

                float aspectRatio = (float) viewport.width / viewport.height;
                float t = tan((fov * M_PI/180) / 2) * nearPlane;
                float r = t * aspectRatio;

                Px *= r;
                Py *= r;

                std::vector<float> direction = {Px, Py, -nearPlane};
                direction = normalize(direction);

                Color rayColor = cast_ray(camPosition, direction);
                glWPoint(x, y, &rayColor);
            }
        }
    }

};