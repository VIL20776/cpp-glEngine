#include <cstdint>
#include <vector>

using namespace std;

using u_char = unsigned char;

// BMP Structs

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

struct ViewPort {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
};

