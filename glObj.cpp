#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <queue>

using namespace std;

using fvector2 = vector<vector<float>>;
using ivector3 = vector<vector<vector<int>>>;

class Obj {
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
        Obj() {
            //reader
            string line;
            ifstream inf("object.obj");
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

        void glLoadModel(string filename) {
            for (auto &&face: faces) {
                int vertCount = 3;
                
                vector<float> v0 = vertexes[ face[0][0] - 1];
                vector<float> v1 = vertexes[ face[0][1] - 1];
                vector<float> v3 = vertexes[ face[0][2] - 1];
            }
        }

        void glCreateObjectMatrix(float translation [3], float rotation [3], float scale [3]) {
            
        }
};