#include "../include/glShaders.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <sys/types.h>
#include <vector>

Color flat (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile * )
{
    const float u = shader_args.at("bCoord_u");
    const float v = shader_args.at("bCoord_v");   
    const float w = shader_args.at("bCoord_w");

    float r = shader_args.at("vColor_r") / 255;
    float g = shader_args.at("vColor_g") / 255;   
    float b = shader_args.at("vColor_b") / 255;

    const std::vector<float> tA = shader_vec_args.at("texCoords_v0");
    const std::vector<float> tB = shader_vec_args.at("texCoords_v1");
    const std::vector<float> tC = shader_vec_args.at("texCoords_v2");

    const std::vector<float> triangleNormal = shader_vec_args.at("triangleNormal");

    if (texture != nullptr) {
        float tU = tA[0] * u + tB[0] * v + tC[0] * w;
        float tV = tA[1] * u + tB[1] * v + tC[1] * w;

        if (tU >= 0 && tV >= 0 && tU < 1 && tV < 1) {
            Color texColor = texture->getColor(tU, tV);

            b *= (float) texColor.B / 255;
            g *= (float) texColor.G / 255;
            r *= (float) texColor.R / 255;
        }
    }
    float intensity = dot(triangleNormal, negateV(dirLight));

    if (intensity <= 0) {
        return {0, 0, 0};
    }

    b *= intensity;
    g *= intensity;
    r *= intensity;

    return {(u_char) (r * 255), (u_char) (g * 255), (u_char) (b * 255)};
}

Color noise (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture,
    BmpFile *normalMap)
{
    const float u = shader_args.at("bCoord_u");
    const float v = shader_args.at("bCoord_v");   
    const float w = shader_args.at("bCoord_w");

    float r = shader_args.at("vColor_r") / 255;
    float g = shader_args.at("vColor_g") / 255;   
    float b = shader_args.at("vColor_b") / 255;

    const std::vector<float> tA = shader_vec_args.at("texCoords_v0");
    const std::vector<float> tB = shader_vec_args.at("texCoords_v1");
    const std::vector<float> tC = shader_vec_args.at("texCoords_v2");
    
    const std::vector<float> nA = shader_vec_args.at("normals_v0");
    const std::vector<float> nB = shader_vec_args.at("normals_v1");
    const std::vector<float> nC = shader_vec_args.at("normals_v2");

    if (texture != nullptr) {
        float tU = tA[0] * u + tB[0] * v + tC[0] * w;
        float tV = tA[1] * u + tB[1] * v + tC[1] * w;

        if (tU >= 0 && tV >= 0 && tU < 1 && tV < 1) {
            Color texColor = texture->getColor(tU, tV);

            b *= (float) texColor.B / 255;
            g *= (float) texColor.G / 255;
            r *= (float) texColor.R / 255;
        }
    }
    
    std::vector<float> triangleNormal {{
        nA[0] * u + nB[0] * v + nC[0] * w,
        nA[1] * u + nB[1] * v + nC[1] * w,
        nA[2] * u + nB[2] * v + nC[2] * w
    }};

    float intensity = dot(triangleNormal, negateV(dirLight));

    if (intensity <= 0) {
        return {0, 0, 0};
    }

    if ((float) rand() / RAND_MAX > 0.5) {
        float random = (float) rand() / RAND_MAX;
        b *= intensity * random;
        g *= intensity * random;
        r *= intensity * random;
    }

    b *= intensity;
    g *= intensity;
    r *= intensity;

    return {(u_char) (r * 255), (u_char) (g * 255), (u_char) (b * 255)};
}

Color light(
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap)
{
    const float u = shader_args.at("bCoord_u");
    const float v = shader_args.at("bCoord_v");   
    const float w = shader_args.at("bCoord_w");

    float r = shader_args.at("vColor_r") / 255;
    float g = shader_args.at("vColor_g") / 255;   
    float b = shader_args.at("vColor_b") / 255;

    const std::vector<float> tA = shader_vec_args.at("texCoords_v0");
    const std::vector<float> tB = shader_vec_args.at("texCoords_v1");
    const std::vector<float> tC = shader_vec_args.at("texCoords_v2");
    
    const std::vector<float> nA = shader_vec_args.at("normals_v0");
    const std::vector<float> nB = shader_vec_args.at("normals_v1");
    const std::vector<float> nC = shader_vec_args.at("normals_v2");

    const std::vector<float> forward = shader_vec_args.at("forwardVector");

    if (texture != nullptr) {
        float tU = tA[0] * u + tB[0] * v + tC[0] * w;
        float tV = tA[1] * u + tB[1] * v + tC[1] * w;

        if (tU >= 0 && tV >= 0 && tU < 1 && tV < 1) {
            Color texColor = texture->getColor(tU, tV);

            b *= (float) texColor.B / 255;
            g *= (float) texColor.G / 255;
            r *= (float) texColor.R / 255;
        }
    }
    
    std::vector<float> triangleNormal {{
        nA[0] * u + nB[0] * v + nC[0] * w,
        nA[1] * u + nB[1] * v + nC[1] * w,
        nA[2] * u + nB[2] * v + nC[2] * w
    }};

    float intensity = dot(triangleNormal, negateV(dirLight));

    if (intensity <= 0) {
        return {0, 0, 0};
    }

    float light = 1 - dot(triangleNormal, forward);
    if (light <= 0) light = 0;

    b += light;
    g += light;
    r += light;

    if (b >= 1) b = 1;
    if (g >= 1) g = 1;
    if (r >= 1) r = 1;

    return {(u_char) (r * 255), (u_char) (g * 255), (u_char) (b * 255)};
}

Color gray(
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap)
{
    const float u = shader_args.at("bCoord_u");
    const float v = shader_args.at("bCoord_v");   
    const float w = shader_args.at("bCoord_w");

    float r = shader_args.at("vColor_r") / 255;
    float g = shader_args.at("vColor_g") / 255;   
    float b = shader_args.at("vColor_b") / 255;

    const std::vector<float> tA = shader_vec_args.at("texCoords_v0");
    const std::vector<float> tB = shader_vec_args.at("texCoords_v1");
    const std::vector<float> tC = shader_vec_args.at("texCoords_v2");
    
    const std::vector<float> nA = shader_vec_args.at("normals_v0");
    const std::vector<float> nB = shader_vec_args.at("normals_v1");
    const std::vector<float> nC = shader_vec_args.at("normals_v2");

    const std::vector<float> forward = shader_vec_args.at("forwardVector");

    if (texture != nullptr) {
        float tU = tA[0] * u + tB[0] * v + tC[0] * w;
        float tV = tA[1] * u + tB[1] * v + tC[1] * w;

        if (tU >= 0 && tV >= 0 && tU < 1 && tV < 1) {
            Color texColor = texture->getColor(tU, tV);

            b *= (float) texColor.B / 255;
            g *= (float) texColor.G / 255;
            r *= (float) texColor.R / 255;
        }
    }
    
    std::vector<float> triangleNormal {{
        nA[0] * u + nB[0] * v + nC[0] * w,
        nA[1] * u + nB[1] * v + nC[1] * w,
        nA[2] * u + nB[2] * v + nC[2] * w
    }};

    float intensity = dot(triangleNormal, negateV(dirLight));

    if (intensity <= 0) {
        return {0, 0, 0};
    }

    std::array<float, 3> color = {r, g, b};
    float hiegherC = *std::max_element(color.begin(), color.end());

    return {(u_char) (hiegherC * 255), (u_char) (hiegherC * 255), (u_char) (hiegherC * 255)};
}

Color RTC (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture,
    BmpFile *normalMap)
{
    const float u = shader_args.at("bCoord_u");
    const float v = shader_args.at("bCoord_v");   
    const float w = shader_args.at("bCoord_w");

    float r = shader_args.at("vColor_r") / 255;
    float g = shader_args.at("vColor_g") / 255;   
    float b = shader_args.at("vColor_b") / 255;

    float yPos = shader_args.at("yPosition");

    const std::vector<float> tA = shader_vec_args.at("texCoords_v0");
    const std::vector<float> tB = shader_vec_args.at("texCoords_v1");
    const std::vector<float> tC = shader_vec_args.at("texCoords_v2");
    
    const std::vector<float> nA = shader_vec_args.at("normals_v0");
    const std::vector<float> nB = shader_vec_args.at("normals_v1");
    const std::vector<float> nC = shader_vec_args.at("normals_v2");

    if (texture != nullptr) {
        float tU = tA[0] * u + tB[0] * v + tC[0] * w;
        float tV = tA[1] * u + tB[1] * v + tC[1] * w;

        if (tU >= 0 && tV >= 0 && tU < 1 && tV < 1) {
            Color texColor = texture->getColor(tU, tV);

            b *= (float) texColor.B / 255;
            g *= (float) texColor.G / 255;
            r *= (float) texColor.R / 255;
        }
    }
    
    std::vector<float> triangleNormal {{
        nA[0] * u + nB[0] * v + nC[0] * w,
        nA[1] * u + nB[1] * v + nC[1] * w,
        nA[2] * u + nB[2] * v + nC[2] * w
    }};

    float intensity = dot(triangleNormal, negateV(dirLight));
    intensity *= std::abs(std::sin(yPos));

    if (intensity <= 0) {
        return {0, 0, 0};
    }

    b *= intensity;
    g *= intensity;
    r *= intensity;

    return {(u_char) (r * 255), (u_char) (g * 255), (u_char) (b * 255)};
}

Color fried(
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap)
{
    const float u = shader_args.at("bCoord_u");
    const float v = shader_args.at("bCoord_v");   
    const float w = shader_args.at("bCoord_w");

    float r = shader_args.at("vColor_r") / 255;
    float g = shader_args.at("vColor_g") / 255;   
    float b = shader_args.at("vColor_b") / 255;

    const std::vector<float> tA = shader_vec_args.at("texCoords_v0");
    const std::vector<float> tB = shader_vec_args.at("texCoords_v1");
    const std::vector<float> tC = shader_vec_args.at("texCoords_v2");
    
    const std::vector<float> nA = shader_vec_args.at("normals_v0");
    const std::vector<float> nB = shader_vec_args.at("normals_v1");
    const std::vector<float> nC = shader_vec_args.at("normals_v2");

    const std::vector<float> forward = shader_vec_args.at("forwardVector");

    if (texture != nullptr) {
        float tU = tA[0] * u + tB[0] * v + tC[0] * w;
        float tV = tA[1] * u + tB[1] * v + tC[1] * w;

        if (tU >= 0 && tV >= 0 && tU < 1 && tV < 1) {
            Color texColor = texture->getColor(tU, tV);

            b *= (float) texColor.B / 255;
            g *= (float) texColor.G / 255;
            r *= (float) texColor.R / 255;
        }
    }
    
    std::vector<float> triangleNormal {{
        nA[0] * u + nB[0] * v + nC[0] * w,
        nA[1] * u + nB[1] * v + nC[1] * w,
        nA[2] * u + nB[2] * v + nC[2] * w
    }};

    float intensity = dot(triangleNormal, negateV(dirLight));

    if (intensity <= 0) {
        return {0, 0, 0};
    }

    b *= intensity;
    g *= intensity;
    r *= intensity;

    b = (b <= 0.5) ? 0.2: 1;
    g = (g <= 0.5) ? 0.2: 1;
    r = (r <= 0.5) ? 0.2: 1;

    return {(u_char) (r * 255), (u_char) (g * 255), (u_char) (b * 255)};
}

//Normal mapping

Color normalMap (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture,
    BmpFile *normalMap)
{
    const float u = shader_args.at("bCoord_u");
    const float v = shader_args.at("bCoord_v");   
    const float w = shader_args.at("bCoord_w");

    float r = shader_args.at("vColor_r") / 255;
    float g = shader_args.at("vColor_g") / 255;   
    float b = shader_args.at("vColor_b") / 255;

    const std::vector<float> tA = shader_vec_args.at("texCoords_v0");
    const std::vector<float> tB = shader_vec_args.at("texCoords_v1");
    const std::vector<float> tC = shader_vec_args.at("texCoords_v2");

    const std::vector<float> nA = shader_vec_args.at("normals_v0");
    const std::vector<float> nB = shader_vec_args.at("normals_v1");
    const std::vector<float> nC = shader_vec_args.at("normals_v2");

    const std::vector<float> tangent = shader_vec_args.at("tangent");
    const std::vector<float> bitangent = shader_vec_args.at("bitangent");

    float tU = tA[0] * u + tB[0] * v + tC[0] * w;
    float tV = tA[1] * u + tB[1] * v + tC[1] * w;

    if (texture != nullptr) {
        if (tU >= 0 && tV >= 0 && tU < 1 && tV < 1) {
            Color texColor = texture->getColor(tU, tV);

            b *= (float) texColor.B / 255;
            g *= (float) texColor.G / 255;
            r *= (float) texColor.R / 255;
        }
    }

    std::vector<float> triangleNormal {{
        nA[0] * u + nB[0] * v + nC[0] * w,
        nA[1] * u + nB[1] * v + nC[1] * w,
        nA[2] * u + nB[2] * v + nC[2] * w
    }};

    float intensity {};
    if (normalMap != nullptr) {
        Color tex = normalMap->getColor(tU, tV);
        std::vector<float> texNormal {{(float) tex.R / 255, (float) tex.G / 255, (float) tex.B / 255}};

        texNormal = {
            (texNormal[0] * 2) - 1,
            (texNormal[1] * 2) - 1,
            (texNormal[2] * 2) - 1
            };

        texNormal = normalize(texNormal);

        Matrix<float, 3, 3> tangentMatrix {{{
            {tangent[0], bitangent[0], triangleNormal[0]},
            {tangent[1], bitangent[1], triangleNormal[1]},
            {tangent[2], bitangent[2], triangleNormal[2]}
        }}};

        texNormal = tangentMatrix * texNormal;
        texNormal = normalize(texNormal);

        intensity = dot(texNormal, negateV(dirLight));
    } else {
        intensity = dot(triangleNormal, negateV(dirLight));
    }

    if (intensity <= 0) {
        return {0, 0, 0};
    }

    b *= intensity;
    g *= intensity;
    r *= intensity;

    return {(u_char) (r * 255), (u_char) (g * 255), (u_char) (b * 255)};
}