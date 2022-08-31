#include "../include/glShaders.hpp"
#include <sys/types.h>

Color flat (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture )
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

            b *= (float) texColor.B;
            g *= (float) texColor.G;
            r *= (float) texColor.R;
        }
    }
    std::vector<float> newdirLight = negateV(dirLight);
    float intensity = dot(triangleNormal, newdirLight);

    if (intensity <= 0) {
        return {0, 0, 0};
    }

    b *= intensity;
    g *= intensity;
    r *= intensity;

    return {(u_char) (r * 255), (u_char) (g * 255), (u_char) (g * 255)};
}