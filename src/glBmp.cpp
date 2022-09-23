#include "../include/glBmp.hpp"
#include <cmath>

// Create base file
BmpFile::BmpFile () {
    // Header
    header.signature[0] = 'B';
    header.signature[1] = 'M';
    header.fileSize = 0;
    header.reserved = 0;
    header.dataOffset = 54;

    // Info header
    info.size = 40;
    info.width = 0;
    info.height = 0;
    info.planes = 1;
    info.bitsPerPixel = 24;
    info.compression = 0;
    info.imageSize = 0;
    info.xPixelsPerM = 0;
    info.yPixelsPerM = 0;
    info.colorsUsed = 0;
    info.importantColors = 0;
}

// Load existing file
BmpFile::BmpFile (const char *fname) {
    std::ifstream inp{ fname, std::ios_base::binary };
    if (inp) {
        inp.read((char*)&header, sizeof(header));
            if(header.signature[0] != 'B' || header.signature[1] != 'M') {
                throw std::runtime_error("Error! Unrecognized file format.");
        }
        inp.read((char*)&info, sizeof(info));

        // Jump to the pixel data location
        inp.seekg(header.dataOffset, inp.beg);

        // Adjust the header fields for output.
        // Some editors will put extra info in the image file, we only save the headers and the data.
        info.size = sizeof(BMPInfoHeader);
        header.dataOffset = sizeof(BMPHeader) + sizeof(BMPInfoHeader);
        
        header.fileSize = header.dataOffset;

        if (info.height < 0) {
            throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
        }

        data.resize(info.width * info.height);

        // Here we check if we need to take into account row padding
        const int padding = ((4 - (info.width * 3) % 4) % 4);

        for (int i = 0; i < (info.width * info.height); i++) {
            std::array<unsigned char, 3> color;
            inp.read((char*) color.data(), 3);

            data[i] = {color[0], color[1], color[2]};
        }
        inp.ignore(padding);
    }
    else {
        throw std::runtime_error("Unable to open the input image file.");
    }
}

void BmpFile::setSize (uint32_t width, uint32_t height)
{
    info.width = width;
    info.height = height;
    info.imageSize = (width * height * 3);
    header.fileSize = 54 + info.imageSize;
}

void BmpFile::setData (std::vector<Color> data)
{
    this->data = data;
}

Color BmpFile::getColor (float u, float v)
{
    return data.at((int) (v * info.height) * info.width + (int) (u * info.width));
}

std::vector<float> BmpFile::getEnvColor(std::vector<float> dir)
{
    dir = normalize(dir);

    int x = int((std::atan2(dir.at(2), dir.at(0)) / (2 * M_PI) + 0.5) * info.width);
    int y = int(std::acos(-dir.at(1)) / M_PI * info.height);

    Color env = data.at(y * info.width + x);
    return {(float) env.R / 255, (float) env.G / 255, (float) env.B / 255};
}

std::vector<Color> BmpFile::getData ()
{
    return data;
}

uint32_t BmpFile::getWidth ()
{
    return info.width;
}

uint32_t BmpFile::getHeight ()
{
    return info.height;
}

bool BmpFile::empty ()
{
    return data.empty();
}

void BmpFile::save ()
{
    std::ofstream of("output.bmp", std::ios_base::binary);
    of.write((const char*) &header, sizeof(header));
    of.write((const char*) &info, sizeof(info));
    of.write((const char*) data.data(), info.imageSize);
    of.close();
}
