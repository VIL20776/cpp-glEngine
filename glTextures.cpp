#include <array>
#include <fstream>

#include "glStructs.cpp"

class Texture {
    private:
    BMPHeader header;
    BMPInfoHeader info;
    std::vector<Color> data;
    
    public:
    Texture (const char *fname) {
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

                data[i] = {color[2], color[1], color[0]};
            }
            inp.ignore(padding);
        }
        else {
            throw std::runtime_error("Unable to open the input image file.");
        }
     }
};