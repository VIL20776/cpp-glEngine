/*
    Structures used by glRender2D and glRender3D
*/
#include <cstdint>
#include <vector>

using u_char = unsigned char;

// BMP Format Structs
#pragma pack(push, 1)
struct BMPHeader {
    u_char signature [2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
};

struct BMPInfoHeader {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t xPixelsPerM;
    uint32_t yPixelsPerM;
    uint32_t colorsUsed;
    uint32_t importantColors;
};
#pragma pack (pop)

// Utility Structs
struct Color {
    u_char R;
    u_char G;
    u_char B;
};

struct DirLight {
    float x;
    float y;
    float z;
};

struct ViewPort {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
};

struct ObjFaceVec{
    std::vector<float> v0;
    std::vector<float> v1;
    std::vector<float> v2;
};