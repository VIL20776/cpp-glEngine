
#include "../include/liblinalg.hpp"

std::vector<float> substract (const std::vector<float> v0, const std::vector<float> v1)
{
    std::vector<float> result {};
    for (int i = 0; i < v0.size(); i++) {
        result.push_back(v0.at(i) - v1.at(i));
    }
    return result;
}

std::vector<float> divide (const std::vector<float> v0, const std::vector<float> v1)
{
    std::vector<float> result {};
    for (int i = 0; i < v0.size(); i++) {
        result.push_back(v0.at(i) / v1.at(i));
    }
    return result;
}

std::vector<float> cross (const std::vector<float> v0, const std::vector<float> v1)
{
    float x = (v0.at(1) * v1.at(2)) - (v0.at(2) * v1.at(1));
    float y = (v0.at(0) * v1.at(2)) - (v0.at(2) * v1.at(0));
    float z = (v0.at(0) * v1.at(1)) - (v0.at(1) * v1.at(0));

    return {x, y, z};
}

std::vector<float> normalize (const std::vector<float> v)
{
    float magnitude {};
    for (auto &i: v)
    {
        magnitude += std::pow(i, 2);
    }
    
    std::vector<float> result {};
    for (auto &i : v) {
        result.push_back(i / magnitude);
    }

    return result;
}

float dot (const std::vector<float> v0, const std::vector<float> v1)
{
    float result;
    for (size_t i = 0; i < v0.size(); i++) {
        result += v0.at(i) * v1.at(i);
    }
    return result;
}

std::vector<float> negateV (const std::vector<float> v)
{
    std::vector<float> result {};
    for (auto &i : v) {
        result.push_back(i);
    }

    return result;
}