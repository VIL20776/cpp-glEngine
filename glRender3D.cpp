#include <array>
#include <math.h>

#include "liblinalg.cpp"
#include "glObj.cpp"
#include "glBmp.cpp"

class GlRender3D: public GlBmp
{

    void glLoadModel(string filename, array<float, 3> translate = {0, 0, 0}, array<float, 3> rotate = {0, 0, 0}, array<float, 3> scale = {0, 0, 0}) {
        Obj model = Obj(filename);
        const Matrix<float, 4, 4> modelMatrix = glCreateObjectMatrix(translate, rotate, scale);

        for (auto &face: model.getFaces()) {
            int vertCount = face.size();
            
            vector<float> t_v0 = model.getVertexes()[ face[0][0] - 1];
            vector<float> t_v1 = model.getVertexes()[ face[1][0] - 1];
            vector<float> t_v2 = model.getVertexes()[ face[2][0] - 1];

            array<float, 3> v0 = glTransform(t_v0, modelMatrix);
            array<float, 3> v1 = glTransform(t_v1, modelMatrix);
            array<float, 3> v2 = glTransform(t_v2, modelMatrix);

            vector<float> vt0 = model.getTextCoords()[ face[0][1] - 1];
            vector<float> vt1 = model.getTextCoords()[ face[1][1] - 1];
            vector<float> vt2 = model.getTextCoords()[ face[2][1] - 1];

            vector<float> vn0 = model.getNormals()[ face[0][2] - 1];
            vector<float> vn1 = model.getNormals()[ face[1][2] - 1];
            vector<float> vn2 = model.getNormals()[ face[2][2] - 1];

            // triangle_bc

            if (vertCount == 4) {
                vector<float> t_v3 = model.getVertexes()[ face[3][0] - 1];
                array<float, 3> v3 = glTransform(t_v3, modelMatrix);
                vector<float> vt3 = model.getTextCoords()[ face[3][1] - 1];
                vector<float> vn3 = model.getNormals()[ face[3][2] - 1];

                // triangle_bc
            }

        }
    }

    Matrix<float, 4, 4> glCreateRotationMatrix (float pitch = 0, float yaw = 0, float roll = 0)
    {
        pitch *= M_PI / 180;
        yaw *= M_PI / 180;
        roll *= M_PI / 180;

        Matrix<float, 4, 4> pitchMat ({{
            {1, 0, 0, 0},
            {0, cos(pitch), -sin(pitch), 0},
            {0, sin(pitch), cos(pitch), 0},
            {0, 0, 0, 1}
        }});

        Matrix<float, 4, 4> yawMat ({{
            {cos(yaw), 0, sin(yaw), 0},
            {0, 1, 0, 0},
            {-sin(yaw), 0, cos(yaw), 0},
            {0, 0, 0, 1}
        }});

        Matrix<float, 4, 4> rollMat ({{
            {cos(roll), -sin(roll), 0, 0},
            {sin(roll), cos(yaw), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }});

        return pitchMat * yawMat * rollMat;
    }

    Matrix<float, 4, 4> glCreateObjectMatrix (array<float, 3> translate = {0, 0, 0}, array<float, 3> rotate = {0, 0, 0}, array<float, 3> scale = {0, 0, 0})
    {
        Matrix<float, 4, 4> translation (
        {{
            {1, 0, 0, translate.at(0)},
            {0, 1, 0, translate.at(1)},
            {0, 0, 1, translate.at(2)},
            {0, 0, 0, 1}
        }});

        Matrix<float, 4, 4> rotation (glCreateRotationMatrix());

        Matrix<float, 4, 4> scalation (
            {{
                {scale.at(0), 0, 0, 0},
                {0, scale.at(1), 0, 0},
                {0, 0, scale.at(2), 0},
                {0, 0, 0, 1}
            }}
        );

        return translation * rotation * scalation;
    }

    array<float, 3> glTransform (const vector<float> vertex, Matrix<float, 4, 4> matrix)
    {
        array<float, 4> v = {{vertex[0], vertex[1], vertex[2], 1}};
        array<float, 4> vt = matrix * v;
        array<float, 3> vf = {{vt[0] / vt[3], vt[1] / vt[3], vt[2] / vt[3]}};

        return vf;
    }
};