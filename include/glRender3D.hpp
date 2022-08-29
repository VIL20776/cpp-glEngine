#ifndef GLRENDER3D_H
#define GLRENDER3D_H

#include <array>
#include <cmath>
#include <cstddef>
#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <vector>
#include <unordered_map>

#include "glBmp.hpp"
#include "liblinalg.hpp"
#include "glObj.hpp"
#include "glBmpRender.hpp"

class GlRender3D: public GlBmpRender
{
    protected:
    // 3D rendering components

    vector<float> zbuffer {};
    BmpFile texture = {};
    std::array<float, 3> (*shader) (
        std::unordered_map<std::string, float>,
        std::unordered_map<string, std::vector<float>>,
        vector<float>,
        BmpFile *);
    vector<float> dirLight {0, 0, -1};

    // Functions

    array<float, 3> baryCoords (vector<float> A, vector<float> B, vector<float> C, array<float, 2> P);

    Matrix<float, 4, 4> glCreateRotationMatrix (float pitch, float yaw, float roll);

    Matrix<float, 4, 4> glCreateObjectMatrix (array<float, 3> translate, array<float, 3> rotate, array<float, 3> scale);

    vector<float> glTransform (const vector<float> vertex, Matrix<float, 4, 4> matrix);

    void glTriangle_bc (vector<float> A, vector<float> B, vector<float> C, 
        ObjFaceVec verts, ObjFaceVec texCoords, ObjFaceVec normals );

    public:
    void glShader (std::array<float, 3> (*shader) (
        std::unordered_map<std::string, float>,
        std::unordered_map<string, std::vector<float>>,
        vector<float>,
        BmpFile *)
    );

    void glTexture (BmpFile texture);

    void glCreateWindow(uint32_t width, uint32_t height);

    void glClear ();

    void glLoadModel(string filename,
        array<float, 3> translate = {0, 0, 0}, array<float, 3> rotate = {0, 0, 0}, array<float, 3> scale = {0, 0, 0});

};

#endif