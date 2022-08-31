#ifndef GLSHADERS_H
#define GLSHADERS_H

#include <array>
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "glBmp.hpp"
#include "liblinalg.hpp"

Color flat (
    const std::unordered_map<std::string, float> shader_args,
    const std::unordered_map<std::string, std::vector<float>> shader_vec_args,
    const std::vector<float> dirLight,
    BmpFile *texture);

#endif