#include "../include/glRender3D.hpp"

    // Functions
    array<float, 3> GlRender3D::baryCoords (vector<float> A, vector<float> B, vector<float> C, array<float, 2> P)
    {
        float areaPBC = (B.at(1) - C.at(1)) * (P.at(0) - C.at(0)) + (C.at(0) - B.at(0)) * (P.at(1) - C.at(1));
        float areaPAC = (C.at(1) - A.at(1)) * (P.at(0) - C.at(0)) + (A.at(0) - C.at(0)) * (P.at(1) - C.at(1));
        float areaABC = (B.at(1) - C.at(1)) * (A.at(0) - C.at(0)) + (C.at(0) - B.at(0)) * (A.at(1) - C.at(1));

        if (round(areaABC) == 0) {
            return {-1, -1, -1};
        }

        float u = areaPBC / areaABC;
        float v = areaPAC / areaABC;
        float w = 1 - u - v;

        return {u, v, w};
    }

    Matrix<float, 4, 4> GlRender3D::glCreateRotationMatrix (float pitch = 0, float yaw = 0, float roll = 0)
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

    Matrix<float, 4, 4> GlRender3D::glCreateObjectMatrix (array<float, 3> translate = {0, 0, 0}, array<float, 3> rotate = {0, 0, 0}, array<float, 3> scale = {0, 0, 0})
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

    vector<float> GlRender3D::glTransform (const vector<float> vertex, Matrix<float, 4, 4> matrix)
    {
        array<float, 4> v = {{vertex[0], vertex[1], vertex[2], 1}};
        array<float, 4> vt = matrix * v;
        vector<float> vf = {{vt[0] / vt[3], vt[1] / vt[3], vt[2] / vt[3]}};

        return vf;
    }

    void GlRender3D::glTriangle_bc (vector<float> A, vector<float> B, vector<float> C, 
        ObjFaceVec verts, ObjFaceVec texCoords, ObjFaceVec normals )
        {
            array<float, 3> Xvals = {A.at(0), B.at(0), C.at(0)};
            array<float, 3> Yvals = {A.at(1), B.at(1), C.at(1)};

            uint32_t minX = std::round(*std::min_element(Xvals.begin(), Xvals.end()));
            uint32_t minY = std::round(*std::min_element(Yvals.begin(), Yvals.end()));
            uint32_t maxX = std::round(*std::max_element(Xvals.begin(), Xvals.end()));
            uint32_t maxY = std::round(*std::max_element(Yvals.begin(), Yvals.end()));

            vector<float> edge1 = substract(verts.v1, verts.v0);
            vector<float> edge2 = substract(verts.v2, verts.v0);

            vector<float> triangleNormal = cross(edge1, edge2);
            triangleNormal = divide(triangleNormal, normalize(triangleNormal));

            vector<float> deltaUV1 = substract(texCoords.v1, texCoords.v0);
            vector<float> deltaUV2 = substract(texCoords.v2, texCoords.v0);
            float f =   1 / (deltaUV1.at(0) * deltaUV2.at(1) - deltaUV2.at(0) * deltaUV1.at(1));

            vector<float> tangent = {
                f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]),
                f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]),
                f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2])};

            tangent = divide(tangent, normalize(tangent));

            vector<float> bitangent = cross(triangleNormal, tangent);
            bitangent = divide(bitangent, normalize(bitangent));

            for (uint32_t x = minX; x == maxX; x++) {
                for (uint32_t y = minY; y == maxY; y++) {
                    
                    array<float, 3> bCoords = baryCoords(A, B, C, {(float) x, (float) y});

                    if (bCoords.at(0) <= 0 && bCoords.at(1) <= 0 && bCoords.at(2) <= 0) {

                        float z = A.at(2) * bCoords.at(0) + B.at(2) * bCoords.at(1) + C.at(2) * bCoords.at(2);

                        if (x <= width && y <= height) {
                            if (z < zbuffer[y * width + x] && abs(z) < 1) {
                                zbuffer[y * width + x] = z;

                                std::unordered_map<string, float> shader_args {};
                                std::unordered_map<string, vector<float>> shader_vec_args {};

                                shader_args["bCoord_u"] = bCoords.at(0);
                                shader_args["bCoord_v"] = bCoords.at(1);
                                shader_args["bCoord_w"] = bCoords.at(2);

                                shader_args["vColor_r"] = point.R;
                                shader_args["vColor_g"] = point.G;
                                shader_args["vColor_b"] = point.B;

                                shader_vec_args["texCoords_v0"] = texCoords.v0;
                                shader_vec_args["texCoords_v1"] = texCoords.v1;
                                shader_vec_args["texCoords_v2"] = texCoords.v2;

                                shader_vec_args["normals_v0"] = normals.v0;
                                shader_vec_args["normals_v1"] = normals.v1;
                                shader_vec_args["normals_v2"] = normals.v2;

                                shader_vec_args["triangleNormal"] = triangleNormal;
                                shader_vec_args["tangent"] = tangent;
                                shader_vec_args["bitangent"] = bitangent;
                                //shader
                                std::array<float, 3> newColor = shader(shader_args, shader_vec_args, dirLight, &texture);
                                glPointColor(newColor.at(0), newColor.at(1), newColor.at(2));
                                glWPoint(x, y);
                            } else {
                                glWPoint(x, y);
                            }
                        }
                    }
                }
            }
        }

    void GlRender3D::glShader (std::array<float, 3> (*shader) (
        std::unordered_map<std::string, float>,
        std::unordered_map<string, std::vector<float>>,
        vector<float>,
        BmpFile *)
    )
    {
        this->shader = shader;
    }

    void GlRender3D::glTexture (BmpFile texture)
    {
        this->texture = texture;
    }

    void GlRender3D::glCreateWindow(uint32_t width, uint32_t height) {
        this->width = width;
        this->height = height;
        pixels.resize(width * height);
    }

    void GlRender3D::glClear ()
    {
        pixels.clear();
        zbuffer.clear();

        size_t gridSize = width * height;
        pixels.assign(gridSize, clear);
        zbuffer.assign(gridSize, INFINITY);
    }

    void GlRender3D::glLoadModel(string filename,
        array<float, 3> translate, array<float, 3> rotate, array<float, 3> scale)
    {
        Obj model = Obj(filename);
        const Matrix<float, 4, 4> modelMatrix = glCreateObjectMatrix(translate, rotate, scale);

        for (auto &face: model.getFaces()) {
            int vertCount = face.size();
            
            vector<float> t_v0 = model.getVertexes()[ face[0][0] - 1];
            vector<float> t_v1 = model.getVertexes()[ face[1][0] - 1];
            vector<float> t_v2 = model.getVertexes()[ face[2][0] - 1];

            vector<float> v0 = glTransform(t_v0, modelMatrix);
            vector<float> v1 = glTransform(t_v1, modelMatrix);
            vector<float> v2 = glTransform(t_v2, modelMatrix);

            vector<float> vt0 = model.getTextCoords()[ face[0][1] - 1];
            vector<float> vt1 = model.getTextCoords()[ face[1][1] - 1];
            vector<float> vt2 = model.getTextCoords()[ face[2][1] - 1];

            vector<float> vn0 = model.getNormals()[ face[0][2] - 1];
            vector<float> vn1 = model.getNormals()[ face[1][2] - 1];
            vector<float> vn2 = model.getNormals()[ face[2][2] - 1];

            glTriangle_bc(v0, v1, v2, 
            {v0, v1, v2}, 
            {vt0, vt1, vt2},
            {vn0, vn1, vn2});

            if (vertCount == 4) {
                vector<float> t_v3 = model.getVertexes()[ face[3][0] - 1];
                vector<float> v3 = glTransform(t_v3, modelMatrix);
                vector<float> vt3 = model.getTextCoords()[ face[3][1] - 1];
                vector<float> vn3 = model.getNormals()[ face[3][2] - 1];

                glTriangle_bc(v0, v1, v2, 
                {v0, v1, v2}, 
                {vt0, vt1, vt2},
                {vn0, vn1, vn2});
            }

        }
    }
    