#include <string>
#include <unordered_map>
#include <vector>

#include "glTextures.cpp"

Color flat (std::unordered_map<std::string, float> shader_args, std::unordered_map<std::string, std::vector<float>> shader_vec_args, Texture texture)
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

    const std::vector<float> triangleNormal = shader_vec_args.at("trinagleNormal");

    



}