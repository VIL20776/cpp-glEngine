#ifndef GLBMP_H
#define GLBMP_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>

#include "glStructs.hpp"

class BmpFile {
    private:
    BMPHeader header;
    BMPInfoHeader info;
    std::vector<Color> data;
    
    public:
    // Create base file
    BmpFile ();

    // Load existing file
    BmpFile (const char *fname);

    void setSize (uint32_t width, uint32_t height);

    void setData (std::vector<Color> data);

    Color getColor (size_t u, size_t v);

    bool empty ();

    void save ();
};

#endif
