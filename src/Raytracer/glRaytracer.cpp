#include "../../include/Raytracer/glRaytracer.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

Intersect globalSceneIntersect(std::vector<float> orig, std::vector<float> dir, Object *sceneObj)
{
    return glrtx->scene_intersect(orig, dir, sceneObj);
}

GlRaytracer::GlRaytracer (uint32_t width, uint32_t height)
{
    glInit();
    glCreateWindow(width, height);
    glViewPort(0, 0, width, height);
    glClear();
    glrtx = this;
}

void GlRaytracer::addLight (Light *light)
{
    lights.push_back(light);
}

void GlRaytracer::addObject (Object *object)
{
    scene.push_back(object);
}

void GlRaytracer::setEnvMap(BmpFile envMap)
{
    this->envMap = envMap;
}

Intersect GlRaytracer::scene_intersect (std::vector<float> orig, std::vector<float> dir, Object *sceneObj)
{
    float depht = INFINITY;
    Intersect intersect {};

    for (auto& object : scene) {
        Intersect hit = object->ray_intersect(orig, dir);
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

std::vector<float> GlRaytracer::cast_ray (
    std::vector<float> orig,
    std::vector<float> dir,
    Object *sceneObj,
    int recursion)
{
    Intersect intersect = scene_intersect(orig, dir, sceneObj);

    if (intersect.null || recursion >= MAX_RECURSION_DEPTH) {
        if (!envMap.empty()) {
            return envMap.getEnvColor(dir);
        } else {
            return {
                (float) this->clear.R / 255,
                (float) this->clear.B / 255,
                (float) this->clear.G / 255,
            };
        }
    }

    Material material = intersect.sceneObj->getMaterial();

    std::vector<float> finalColor = {0, 0, 0};
    std::vector<float> objectColor = {
        material.diffuse.at(0),
        material.diffuse.at(1),
        material.diffuse.at(2)};
    
    switch (material.type) {
            case 0: //OPAQUE
                for (auto &light: lights)
                {
                    std::vector<float> diffuseColor = light->getDiffuseColor(intersect);
                    std::vector<float> specColor = light->getSpecColor(intersect, &camPosition);
                    Intersect (*sIntersect)(std::vector<float>, std::vector<float>, Object *) { globalSceneIntersect };
                    float shadow_intensity = light->getShadowIntensity(intersect, sIntersect);

                    std::vector<float> lightColor = mult((1 - shadow_intensity), add(diffuseColor, specColor));

                    finalColor = add(finalColor, lightColor);
                }
                break;
            
            case 1: //REFLECTIVE
            {
                std::vector<float> reflect = reflectVector(intersect.normal, mult(-1, dir));
                vector<float> reflectColor = cast_ray(intersect.point, reflect, intersect.sceneObj, recursion + 1);

                std::vector<float> specColor {0, 0, 0};
                for (auto &light: lights)
                {
                    specColor = add(specColor, light->getSpecColor(intersect, &camPosition));
                }
                finalColor = add(reflectColor, specColor);
                break;
            }

            case 2: //TRANSPARENT
            {
                bool outside = dot(dir, intersect.normal) < 0;
                std::vector<float> bias = mult(0.001, intersect.normal);

                std::vector<float> specColor = {0, 0, 0};
                for (auto &light : lights) {
                    specColor = add(specColor, light->getSpecColor(intersect, &camPosition));
                }

                std::vector<float> reflect = reflectVector(intersect.normal, mult(-1, dir));
                std::vector<float> reflectOrig = outside ? add(intersect.point, bias): substract(intersect.point, bias);
                vector<float> reflectColor = cast_ray(intersect.point, reflect, intersect.sceneObj, recursion + 1);

                float kr = fresnel(intersect.normal, dir, material.ir);

                if (kr >= 1) break;

                std::vector<float> refract = refractVector(intersect.normal, dir, material.ir);
                std::vector<float> refractOrig = !outside ? add(intersect.point, bias): substract(intersect.point, bias);
                vector<float> refractColor = cast_ray(intersect.point, reflect, nullptr, recursion + 1);
                finalColor = add(add(mult(kr, refractColor), mult((1 - kr), refractColor)), specColor);
                break;
            }
                
        }
    finalColor = mult(finalColor, objectColor);

    if (!material.texture.empty() && !intersect.texCoords.empty()) {
        vector<float> texColor = material.texture.getColor(intersect.texCoords.at(0), intersect.texCoords.at(1));
        if (!texColor.empty())
            finalColor = mult(finalColor, texColor);
    }

    float r = std::min((float) 1, finalColor.at(0));
    float g = std::min((float) 1, finalColor.at(1));
    float b = std::min((float) 1, finalColor.at(2));

    return {r, g, b};
}

void GlRaytracer::glLoadModel(string filename,
    array<float, 3> translate, array<float, 3> rotate, array<float, 3> scale,
    Material material)
{
    std::vector<Triangle> polygons {};

    Obj model = Obj(filename);
    const Matrix<float, 4, 4> modelMatrix = glCreateObjectMatrix(translate, rotate, scale);
    const Matrix<float, 4, 4> rotationMatrix = glCreateRotationMatrix(rotate.at(0), rotate.at(1), rotate.at(2));

    //#pragma omp parallel for
    for (auto &face: model.getFaces()) {
        int vertCount = face.size();
        
        vector<float> t_v0 = model.getVertexes().at( face[0][0] - 1);
        vector<float> t_v1 = model.getVertexes().at( face[1][0] - 1);
        vector<float> t_v2 = model.getVertexes().at( face[2][0] - 1);

        vector<float> v0 = glTransform(t_v0, modelMatrix);
        vector<float> v1 = glTransform(t_v1, modelMatrix);
        vector<float> v2 = glTransform(t_v2, modelMatrix);

        vector<float> A = glCamTransform(v0);
        vector<float> B = glCamTransform(v1);
        vector<float> C = glCamTransform(v2);

        polygons.push_back(Triangle (A, B, C, material));

        if (vertCount == 4) {
            vector<float> t_v3 = model.getVertexes().at( face[3][0] - 1);
            vector<float> v3 = glTransform(t_v3, modelMatrix);
            vector<float> D = glCamTransform(v3);

            polygons.push_back(Triangle (A, C, D, material));
        }
    }

    models.push_back( Model (polygons, material) );
}

void GlRaytracer::glRender ()
{
    if (!models.empty()) {
        for (auto &model : models) {
            scene.push_back(&model);
        }
    }

    float aspectRatio = (float) viewport.width / (float) viewport.height;
    float t = tan((fov * M_PI/180) / 2) * nearPlane;
    float r = t * aspectRatio;

    for (size_t y = viewport.y; y < (viewport.y + viewport.height + 1); y++) {
        for (size_t x = viewport.x; x < (viewport.x + viewport.width + 1); x++) {
            
            float Px = (((x + 0.5 - (float) viewport.x) / (float) viewport.width) * 2) - 1;
            float Py = (((y + 0.5 - (float) viewport.y) / (float) viewport.height) * 2) - 1;

            Px *= r;
            Py *= r;

            std::vector<float> direction = {Px, Py, -nearPlane};
            direction = normalize(direction);

            std::vector<float> rayArray = cast_ray(camPosition, direction);

            if (rayArray.empty()) {
                continue;
            }
            
            Color rayColor {
                u_char (rayArray[0] * 255),
                u_char (rayArray[1] * 255),
                u_char (rayArray[2] * 255)
            };
            glWPoint(x, y, &rayColor);
        }
    }
}