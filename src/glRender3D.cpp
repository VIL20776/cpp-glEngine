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

void GlRender3D::glViewPort(float x, float y, float width, float height)
{
    viewport = {(uint32_t) x, (uint32_t) y, (uint32_t) width, (uint32_t) height};
    viewportMatrix = {{{
        {width/2, 0, 0, x + (width/2)},
        {0, height/2, 0, y + (height/2)},
        {0, 0, 0.5, 0.5},
        {0, 0, 0, 1}
    }}};

    glProjectionMatrix();
}

void GlRender3D::glViewMatrix (array<float, 3> translate, array<float, 3> rotate)
{
    camMatrix = glCreateObjectMatrix(translate, rotate);
    viewMatrix = camMatrix.inverse();
}

void GlRender3D::glProjectionMatrix (float n, float f, float fov)
{
    float aspectRatio = (float) viewport.width / viewport.height;
    float t = tan((fov * M_PI/180) / 2) * n;
    float r = t * aspectRatio;

    projectionMatrix = {{{
        {n/r, 0, 0, 0},
        {0, n/t, 0, 0},
        {0, 0, -(f+n)/(f-n), -(2*f*n)/(f-n)},
        {0, 0, -1, 0}
    }}};
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
        {sin(roll), cos(roll), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    }});

    return pitchMat * (yawMat * rollMat);
}

Matrix<float, 4, 4> GlRender3D::glCreateObjectMatrix (array<float, 3> translate, array<float, 3> rotate, array<float, 3> scale)
{
    Matrix<float, 4, 4> translation (
    {{
        {1, 0, 0, translate.at(0)},
        {0, 1, 0, translate.at(1)},
        {0, 0, 1, translate.at(2)},
        {0, 0, 0, 1}
    }});

    Matrix<float, 4, 4> rotation (glCreateRotationMatrix(rotate[0], rotate[1], rotate[2]));

    Matrix<float, 4, 4> scalation (
        {{
            {scale.at(0), 0, 0, 0},
            {0, scale.at(1), 0, 0},
            {0, 0, scale.at(2), 0},
            {0, 0, 0, 1}
        }}
    );

    return translation * (rotation * scalation);
}

vector<float> GlRender3D::glTransform (const vector<float> vertex, Matrix<float, 4, 4> matrix)
{
    vector<float> v = {{vertex[0], vertex[1], vertex[2], 1}};
    vector<float> vt = matrix * v;
    vector<float> vf = {{vt[0] / vt[3], vt[1] / vt[3], vt[2] / vt[3]}};

    return vf;
}

vector<float> GlRender3D::glDirTransform (const vector<float> dirVector, Matrix<float, 4, 4> rotMatrix)
{
    vector<float> v = {{dirVector[0], dirVector[1], dirVector[2], 0}};
    vector<float> vt = rotMatrix * v;
    vector<float> vf = {{vt[0], vt[1], vt[2]}};

    return vf;
}

vector<float> GlRender3D::glCamTransform (const vector<float> vertex)
{
    vector<float> v = {{vertex[0], vertex[1], vertex[2], 1}};
    vector<float> vt = (viewportMatrix * (projectionMatrix * (viewMatrix * v)));
    vector<float> vf = {{vt[0] / vt[3], vt[1] / vt[3], vt[2] / vt[3]}};

    return vf;
}


void GlRender3D::glTriangle_bc (vector<float> A, vector<float> B, vector<float> C, 
    ObjFaceVec verts, ObjFaceVec texCoords, ObjFaceVec normals )
{
    array<float, 3> Xvals = {A.at(0), B.at(0), C.at(0)};
    array<float, 3> Yvals = {A.at(1), B.at(1), C.at(1)};

    long minX = (long) std::round(*std::min_element(Xvals.begin(), Xvals.end()));
    long minY = (long) std::round(*std::min_element(Yvals.begin(), Yvals.end()));
    long maxX = (long) std::round(*std::max_element(Xvals.begin(), Xvals.end()));
    long maxY = (long) std::round(*std::max_element(Yvals.begin(), Yvals.end()));

    vector<float> edge1 = substract(verts.v1, verts.v0);
    vector<float> edge2 = substract(verts.v2, verts.v0);

    vector<float> triangleNormal = cross(edge1, edge2);
    triangleNormal = normalize(triangleNormal);

    vector<float> deltaUV1 = substract(texCoords.v1, texCoords.v0);
    vector<float> deltaUV2 = substract(texCoords.v2, texCoords.v0);
    float f =   1 / (deltaUV1.at(0) * deltaUV2.at(1) - deltaUV2.at(0) * deltaUV1.at(1));

    vector<float> tangent = {
        f * (deltaUV2.at(1) * edge1.at(0) - deltaUV1.at(1) * edge2.at(0)),
        f * (deltaUV2.at(1) * edge1.at(1) - deltaUV1.at(1) * edge2.at(1)),
        f * (deltaUV2.at(1) * edge1.at(2) - deltaUV1.at(1) * edge2.at(2))};

    tangent = normalize(tangent);

    vector<float> bitangent = cross(triangleNormal, tangent);
    bitangent = normalize(bitangent);

    for (long x = minX; x <= maxX; x++) {
        for (long y = minY; y <= maxY; y++) {
            
            if (x < width && y < height && x >= 0 && y >= 0) {
                array<float, 3> bCoords = baryCoords(A, B, C, {(float) x, (float) y});

                if (bCoords.at(0) >= 0 && bCoords.at(1) >= 0 && bCoords.at(2) >= 0) {

                    float z = A.at(2) * bCoords.at(0) + B.at(2) * bCoords.at(1) + C.at(2) * bCoords.at(2);

                    if (z < zbuffer.at(y * width + x) && abs(z) <= 1) {
                        zbuffer.at(y * width + x) = z;

                        if (!texture.empty()){
                            std::unordered_map<string, float> shader_args {};
                            std::unordered_map<string, vector<float>> shader_vec_args {};

                            shader_args["bCoord_u"] = bCoords.at(0);
                            shader_args["bCoord_v"] = bCoords.at(1);
                            shader_args["bCoord_w"] = bCoords.at(2);

                            shader_args["vColor_r"] = (float) point.R;
                            shader_args["vColor_g"] = (float) point.G;
                            shader_args["vColor_b"] = (float) point.B;

                            shader_args["yPosition"] = y;

                            shader_vec_args["texCoords_v0"] = texCoords.v0;
                            shader_vec_args["texCoords_v1"] = texCoords.v1;
                            shader_vec_args["texCoords_v2"] = texCoords.v2;

                            shader_vec_args["normals_v0"] = normals.v0;
                            shader_vec_args["normals_v1"] = normals.v1;
                            shader_vec_args["normals_v2"] = normals.v2;

                            shader_vec_args["triangleNormal"] = triangleNormal;
                            shader_vec_args["forwardVector"] = {
                                camMatrix.at(0, 2),
                                camMatrix.at(1, 2),
                                camMatrix.at(2, 2)};
                            shader_vec_args["tangent"] = tangent;
                            shader_vec_args["bitangent"] = bitangent;
                            //shader
                            BmpFile *normalMap = (this->normalMap.empty()) ? nullptr : &this->normalMap;
                            Color newColor = shader(shader_args, shader_vec_args, dirLight, &texture, normalMap);
                            glWPoint(x, y, &newColor);
                        } else {
                            glWPoint(x, y);
                        }
                    }
                }
            }
        }
    }
}

/* public implementations */

void GlRender3D::glInit()
{
    clear = {0, 0, 0};
    point = {255, 255, 255};
    fill = {255, 255, 0};

    glViewMatrix();
}

void GlRender3D::glShader (Color (*shader) (
    std::unordered_map<std::string, float>,
    std::unordered_map<string, std::vector<float>>,
    vector<float>,
    BmpFile *,
    BmpFile *)
)
{
    this->shader = shader;
}

void GlRender3D::glTexture (BmpFile texture)
{
    this->texture = texture;
}

void GlRender3D::glNormalMap(BmpFile normalMap)
{
    this->normalMap = normalMap;
}

void GlRender3D::glLookAt(vector<float> eye, vector<float> camPosition)
{
    vector<float> forward = substract(camPosition, eye);
    forward = normalize(forward);

    vector<float> right = cross({0, 1, 0}, forward);
    right = normalize(right);

    vector<float> up = cross(forward, right);
    up = normalize(up);

    camMatrix = {{{
        {right.at(0), up.at(0), forward.at(0), camPosition.at(0)},
        {right.at(1), up.at(1), forward.at(1), camPosition.at(1)},
        {right.at(2), up.at(2), forward.at(2), camPosition.at(2)},
        {0, 0, 0, 1}
    }}};

    viewMatrix = camMatrix.inverse();
}

void GlRender3D::glCreateWindow(float width, float height) {
    this->width = (uint32_t) width;
    this->height = (uint32_t) height;
    pixels.resize((size_t) width * height);
    zbuffer.resize((size_t) width * height);
    glViewPort(0, 0,width, height);
}


void GlRender3D::glBackground(BmpFile file)
{
    glCreateWindow(file.getWidth(), file.getHeight());

    this->pixels = file.getData();
    zbuffer.assign((width * height), INFINITY);
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
    const Matrix<float, 4, 4> rotationMatrix = glCreateRotationMatrix(rotate.at(0), rotate.at(1), rotate.at(2));

    //#pragma omp parallel for
    for (auto &face: model.getFaces()) {
        int vertCount = face.size();
        
        vector<float> t_v0 = model.getVertexes().at( face[0][0] - 1);
        vector<float> t_v1 = model.getVertexes().at( face[1][0] - 1);
        vector<float> t_v2 = model.getVertexes().at( face[2][0] - 1);

        vector<float> v0 = glTransform(t_v0, modelMatrix);
        vector<float> v1 = glTransform(t_v1, modelMatrix);
        vector<float> v2 = glTransform(t_v2, modelMatrix);

        vector<float> A = glCamTransform(v0);
        vector<float> B = glCamTransform(v1);
        vector<float> C = glCamTransform(v2);
        
        vector<float> vt0 = model.getTextCoords().at( face[0][1] - 1);
        vector<float> vt1 = model.getTextCoords().at( face[1][1] - 1);
        vector<float> vt2 = model.getTextCoords().at( face[2][1] - 1);

        vector<float> vn0 = model.getNormals().at( face[0][2] - 1);
        vector<float> vn1 = model.getNormals().at( face[1][2] - 1);
        vector<float> vn2 = model.getNormals().at( face[2][2] - 1);
        vn0 = glDirTransform(vn0, rotationMatrix);
        vn1 = glDirTransform(vn1, rotationMatrix);
        vn2 = glDirTransform(vn2, rotationMatrix);

            glTriangle_bc(A, B, C, 
            {v0, v1, v2}, 
            {vt0, vt1, vt2},
            {vn0, vn1, vn2});

        if (vertCount == 4) {
            vector<float> t_v3 = model.getVertexes().at( face[3][0] - 1);
            vector<float> v3 = glTransform(t_v3, modelMatrix);
            vector<float> D = glCamTransform(v3);
            vector<float> vt3 = model.getTextCoords().at( face[3][1] - 1);
            vector<float> vn3 = model.getNormals().at( face[3][2] - 1);
            vn3 = glDirTransform(vn3, rotationMatrix);

            glTriangle_bc(A, C, D, 
            {v0, v2, v3}, 
            {vt0, vt2, vt3},
            {vn0, vn2, vn3});
        }
    }
}
    