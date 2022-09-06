#ifndef GLSHADERS_H
#define GLSHADERS_H

#include <array>
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>

#include "glBmp.hpp"
#include "glStructs.hpp"
#include "liblinalg.hpp"

Color flat (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap);

Color noise (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap);

Color normalMap (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap
);

Color light (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap
);

Color gray (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap
);

Color RTC(
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap
);

Color fried(
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture, BmpFile *normalMap
);
#endif