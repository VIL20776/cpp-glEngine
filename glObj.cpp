#include <cmath>
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <array>
#include <math.h>

using namespace std;

class Obj {
    using fvector2 = vector<vector<float>>;
    using ivector3 = vector<vector<vector<int>>>;

    private:
    fvector2 vertexes;
    fvector2 textureCoords;
    fvector2 normals;
    ivector3 faces;

    //Functions
    queue<string> splitLine(string s, string delimiter) {
        size_t pos = 0;
        queue<string> tokens;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            tokens.push(s.substr(0, pos));
            s.erase(0, pos + delimiter.length());
        }
        return tokens;
    }

    public:
    Obj(string filename) {
        //reader
        string line;
        ifstream inf(filename);
        while (!inf.eof()) {
            getline(inf, line);
        
            queue<string> qLine = splitLine(line, " ");

            string prefix = qLine.back();
            qLine.pop();

            if (prefix == "v") {
                vector<float> vertex = {};
                while (qLine.empty()) {
                    vertex.push_back(stof(qLine.back()));
                    qLine.pop();
                }
                vertexes.push_back(vertex);
                continue;
            }
            if (prefix == "vt") {
                vector<float> textureCoord = {};
                while (qLine.empty()) {
                    textureCoord.push_back(stof(qLine.back()));
                    qLine.pop();
                }
                textureCoords.push_back(textureCoord);
                continue;
            }
            if (prefix == "vn") {
                vector<float> normal = {};
                while (qLine.empty()) {
                    normal.push_back(stof(qLine.back()));
                    qLine.pop();
                }
                normals.push_back(normal);
                continue;
            }
            if (prefix == "f") {
                vector<vector<int>> faces;
                while (qLine.empty()) {
                    queue<string> qface = splitLine(qLine.back(), "/");
                    vector<int> face = {};
                    while (qface.size()) {
                        face.push_back(stoi(qface.back()));
                        qface.pop();
                    }
                    faces.push_back(face);
                    qLine.pop();
                }
                this->faces.push_back(faces);
                continue;
            }

        }
    }

    fvector2 getVertexes () { return vertexes; }

    fvector2 getTextCoords () { return textureCoords; }

    fvector2 getNormals () { return normals; }

    ivector3 getFaces () { return faces; }
};