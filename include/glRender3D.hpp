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
    BmpFile normalMap = {};
    Color (*shader) (
        std::unordered_map<std::string, float>,
        std::unordered_map<string, std::vector<float>>,
        vector<float>,
        BmpFile *, BmpFile *);
    vector<float> dirLight {0, 0, -1};
    Matrix<float, 4, 4> camMatrix;
    Matrix<float, 4, 4> viewMatrix;
    Matrix<float, 4, 4> viewportMatrix;
    Matrix<float, 4, 4> projectionMatrix;

    // Functions

    array<float, 3> baryCoords (vector<float> A, vector<float> B, vector<float> C, array<float, 2> P);
    
    void glViewPort(float x, float y, float width, float height);

    void glViewMatrix (array<float, 3> translate = {0, 0, 0}, array<float, 3> rotate = {0, 0, 0});

    void glProjectionMatrix (float n = 0.1, float f = 1000, float fov = 60);

    Matrix<float, 4, 4> glCreateRotationMatrix (float pitch, float yaw, float roll);

    Matrix<float, 4, 4> glCreateObjectMatrix (array<float, 3> translate = {0, 0, 0}, array<float, 3> rotate = {0, 0, 0}, array<float, 3> scale = {1, 1, 1});

    vector<float> glTransform (const vector<float> vertex, Matrix<float, 4, 4> matrix);

    vector<float> glDirTransform (const vector<float> dirVector, Matrix<float, 4, 4> rotMatrix);
    
    vector<float> glCamTransform (const vector<float> vertex);

    void glTriangle_bc (vector<float> A, vector<float> B, vector<float> C, 
        ObjFaceVec verts, ObjFaceVec texCoords, ObjFaceVec normals );

    public:

    void glInit ();

    void glShader (Color (*shader) (
        std::unordered_map<std::string, float>,
        std::unordered_map<string, std::vector<float>>,
        vector<float>,
        BmpFile *,
        BmpFile *)
    );

    void glTexture (BmpFile texture);

    void glNormalMap (BmpFile normalMap);

    void glLookAt (vector<float> eye, vector<float> camPosition = {0, 0, 0});

    void glCreateWindow(float width, float height);

    void glBackground(BmpFile file);

    void glClear ();

    void glLoadModel(string filename,
        array<float, 3> translate = {0, 0, 0}, array<float, 3> rotate = {0, 0, 0}, array<float, 3> scale = {0, 0, 0});

};

#endif
