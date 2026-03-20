#include "octree.hpp"


struct Vertex {
    float x, y, z;
};

struct Face {
    int v1, v2, v3;
};

void parseOBJ(const string& path, vector<Vertex>& vertices, vector<Face>& faces) {
    ifstream file(path);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string type;
        ss >> type;

        if (type == "v") {
            Vertex v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (type == "f") {
            Face f;
            string s1, s2, s3;
            ss >> s1 >> s2 >> s3;
            
            f.v1 = stoi(s1.substr(0, s1.find("/")));
            f.v2 = stoi(s2.substr(0, s2.find("/")));
            f.v3 = stoi(s3.substr(0, s3.find("/")));
            faces.push_back(f);
        }
    }
    file.close();
}
void getVoxelMinMax(vector<Vertex>& vertices,double minX,double minY,double minZ, double maxX,double maxY,double maxZ){
    if (!vertices.empty()){
        minX = vertices[0].x;
        maxX = vertices[0].x;
        minY = vertices[0].y;
        maxY = vertices[0].y;
        minZ = vertices[0].z;
        maxZ = vertices[0].z;
        for (const auto& v : vertices) {
            if (v.x < minX) minX = v.x;
            if (v.x > maxX) maxX = v.x;
            if (v.y < minY) minY = v.y; 
            if (v.y > maxY) maxY = v.y;
            if (v.z < minZ) minZ = v.z; 
            if (v.z > maxZ) maxZ = v.z;
        }
    }
}

Octree voxelarrToOctTree( vector<Face>& faces,vector<Vertex>& vertices){
    double minX,minY,minZ,maxX,maxY,maxZ;
    Octree Otree(minX,minY,minZ,maxX,maxY,maxZ);
    for (const auto& v : vertices) {
        Otree.insert(v.x, v.y, v.z);
    }
    return Otree;
}