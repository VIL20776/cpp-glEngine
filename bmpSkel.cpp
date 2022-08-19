#include <stdio.h>
#include <vector>

using namespace std;

using uchar = unsigned char;
using ushort = unsigned short int;
using uint = unsigned int;

struct BMPHeader {
    uchar signature [2];
    uint fileSize;
    uint reserved;
    uint dataOffset;
};

struct BMPInfoHeader {
    uint size;
    uint width;
    uint height;
    ushort planes;
    ushort bitsPerPixel;
    uint compression;
    uint imageSize;
    uint xPixelsPerM;
    uint yPixelsPerM;
    uint colorsUsed;
    uint importantColors;
};

struct Color {
    uchar R;
    uchar G;
    uchar B;
};

struct BMP {
    BMPHeader header;
    BMPInfoHeader infoHeader;
    vector <Color> pixels;
};