#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <type_traits>
#include <vector>


#include "glStructs.cpp"

using namespace std;

class GlBmp {
    private:
        // BMP components
        BMPHeader header;
        BMPInfoHeader info;
        vector<Color> pixels {};

        // Color Pallete
        Color clear;
        Color point;
        Color fill;

        // Utilities
        ViewPort viewport;
        
        // Functions
        bool evenOdd(uint32_t x, uint32_t y, float poly [][2], size_t num) {
            uint32_t j = num - 1;
            bool c = false;
            for (int i = 0; i < num; i++) {
                if (x == poly[i][0] && y == poly[i][1]) {
                    return true;
                }
                if ((poly[i][1] > y) != (poly[j][1] > y)) {
                    int slope = ((x-poly[i][0])*(poly[j][1]-poly[i][1]))-((poly[j][0]-poly[i][0])*(y-poly[i][1]));
                    if (slope == 0) {
                        return true;
                    }
                    if ((slope < 0) != (poly[j][1] < poly[i][1])) {
                        c = !c;
                    }
                }
                j = i;
            }
            return c;
        }

    public:
        void glInit() {
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

            // Color palette
            clear = {0, 0, 0};
            point = {255, 255, 255};
            fill = {255, 255, 0};
        }

        void glCreateWindow(uint32_t width, uint32_t height) {
            info.width = width;
            info.height = height;
            info.imageSize = (width * height * 3);
            header.fileSize = 54 + info.imageSize;
        }

        void glViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            viewport = {
                x, y,
                width, height
            };
        }

        void glClear() {
            pixels.clear();
            size_t gridSize = info.width * info.height;
            pixels.assign(gridSize, clear);
        }

        void glClearColor(float r, float g, float b) {
            if (abs(r) > 1 || abs(g) > 1 ||abs(b) > 1 ) {
                return;
            }
            clear = {
                (u_char) (r * 255),
                (u_char) (g * 255),
                (u_char) (b * 255)
            };
        }

        void glWPoint(uint32_t x, uint32_t y) {
            if (x < info.width && y < info.height) {
                pixels[y * info.width + x] = point;
            }
        }

        void glVPoint(float x, float y) {
            if (x >= -1 || x <= 1 && y >= -1 || y <= 1) {
                uint32_t newX = (uint32_t) (x + 1) * (viewport.x / 2) + viewport.x;
                uint32_t newY = (uint32_t) (y + 1) * (viewport.y / 2) + viewport.y;

                glWPoint(newX, newY);
            }
        }

        void glPointColor(float r, float g, float b) {
            if (abs(r) > 1 || abs(g) > 1 ||abs(b) > 1 ) {
                return;
            }
            point = {
                (u_char) (r * 255),
                (u_char) (g * 255), 
                (u_char) (b * 255)
                };
        }

        void glLine(float v0 [], float v1 []) {
            float x0 = v0[0]; float x1 = v1[0];
            float y0 = v0[1]; float y1 = v1[1];

            if (x0 == x1 && y0 == y1) {
                glWPoint(x0, y0);
                return;
            }

            float dy = abs(y1 - y0);
            float dx = abs(x1 - x0);

            bool steep = dy > dx;

            if (steep) {
                float temp = x0;
                x0 = y0;
                y0 = temp;

                temp = x1;
                x1 = y1;
                y1 = temp;
            }
            
            if (x0 > x1) {
                float temp = x0;
                x0 = x1;
                x1 = temp;

                temp = y0;
                y0 = y1;
                y1 = temp;
            }

            dy = abs(y1 - y0);
            dx = abs(x1 - x0);

            float offset = 0;
            float limit = 0.5;
            float m = dy / dx;
            float y = y0;

            for (int x = x0; x < (x1 + 1); x++) {
                if (steep) {
                    glWPoint((uint32_t) y, (uint32_t) x);
                } else {
                    glWPoint((uint32_t) x, (uint32_t) y);
                }
                offset += m;

                if (offset >= limit) {
                    if (y0 < y1) {
                        y += 1;
                    } else {
                        y -= 1;
                    }
                    limit += 1;
                }
            }
        }

        void glDraw(float poligon [][2], size_t size) {
            for (int i = 0; i < size; i++) {
                glLine(poligon[i], poligon[ (i + 1) % size]);
            }
        }

        void glFill(float poly[][2], size_t size) {
            for (int x = 0; x < info.width; x++) {
                for (int y = 0; y < info.height; y++) {
                    if(evenOdd(x, y, poly, size)) {
                        pixels[y * info.width + x] = fill;
                    }
                }
            }
        }

        void glFinish() {
            ofstream of("output.bmp", ios_base::binary);
            of.write((const char*) &header, sizeof(header));
            of.write((const char*) &info, sizeof(info));
            of.write((const char*) pixels.data(), info.imageSize);
            of.close();
        }
};

