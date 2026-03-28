#include <bits/stdc++.h>
#include <cstdlib>
#include "octree.hpp"

using namespace std;
using namespace std::chrono;

struct Vertex {
    double x, y, z;
};

struct Face {
    int v1, v2, v3;
};

void parseOBJ(const string& path, vector<Vertex>& vertices, vector<Face>& faces) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Gagal membuka file: " << path << endl;
        return;
    }

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
            string s[3];
            ss >> s[0] >> s[1] >> s[2];
            
            auto getIndex = [](string str) {
                return stoi(str.substr(0, str.find('/')));
            };

            f.v1 = getIndex(s[0]);
            f.v2 = getIndex(s[1]);
            f.v3 = getIndex(s[2]);
            faces.push_back(f);
        }
    }
    file.close();
}

void getVoxelMinMax(const vector<Vertex>& vertices, double& minX, double& minY, double& minZ, double& maxX, double& maxY, double& maxZ){
    if (vertices.empty()) return;
    minX = maxX = vertices[0].x;
    minY = maxY = vertices[0].y;
    minZ = maxZ = vertices[0].z;

    for (const auto& v : vertices) {
        minX = min(minX, v.x); maxX = max(maxX, v.x);
        minY = min(minY, v.y); maxY = max(maxY, v.y);
        minZ = min(minZ, v.z); maxZ = max(maxZ, v.z);
    }
}

void writeVoxelOBJ(const string& path, const vector<AABB>& voxels) {
    ofstream file(path);
    if (!file.is_open()) return;

    int vertexOffset = 1;
    for (const auto& box : voxels) {
        file << "v " << box.min.x << " " << box.min.y << " " << box.min.z << "\n";
        file << "v " << box.max.x << " " << box.min.y << " " << box.min.z << "\n";
        file << "v " << box.max.x << " " << box.max.y << " " << box.min.z << "\n";
        file << "v " << box.min.x << " " << box.max.y << " " << box.min.z << "\n";
        file << "v " << box.min.x << " " << box.min.y << " " << box.max.z << "\n";
        file << "v " << box.max.x << " " << box.min.y << " " << box.max.z << "\n";
        file << "v " << box.max.x << " " << box.max.y << " " << box.max.z << "\n";
        file << "v " << box.min.x << " " << box.max.y << " " << box.max.z << "\n";
        int f[12][3] = {
            {1,2,3}, {1,3,4}, {5,8,7}, {5,7,6}, 
            {1,5,6}, {1,6,2}, {4,3,7}, {4,7,8}, 
            {1,4,8}, {1,8,5}, {2,6,7}, {2,7,3}
        };

        for (int i = 0; i < 12; i++) {
            file << "f " << f[i][0] + vertexOffset - 1 << " " 
                         << f[i][1] + vertexOffset - 1 << " " 
                         << f[i][2] + vertexOffset - 1 << "\n";
        }
        vertexOffset += 8;
    }
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input.obj> [output.obj]" << endl;
        return 1;
    }

    string inputPath = argv[1];
    string outputPath = (argc > 2)?argv[2] : "../test/output.obj";\

    string depthArg = (argc > 3 && atoi(argv[3])<20)?argv[3] : "5";
    string command = "blender --python src/import_obj.py";
    vector<Vertex> vertices;
    vector<Face> faces;

    cout << "[Parsing file obj] : " << inputPath << endl;
    parseOBJ(inputPath, vertices, faces);

    if (vertices.empty()) return 1;

    double minX, minY, minZ, maxX, maxY, maxZ;
    getVoxelMinMax(vertices, minX, minY, minZ, maxX, maxY, maxZ);
    
    int maxDepth = (atoi(depthArg.c_str()));
    vector<Triangle> triangles;
    for (const auto& f : faces) {
        triangles.push_back({
            {vertices[f.v1-1].x, vertices[f.v1-1].y, vertices[f.v1-1].z},
            {vertices[f.v2-1].x, vertices[f.v2-1].y, vertices[f.v2-1].z},
            {vertices[f.v3-1].x, vertices[f.v3-1].y, vertices[f.v3-1].z}
        });
    }

    auto start = high_resolution_clock::now();
    Octree tree(minX, minY, minZ, maxX, maxY, maxZ);
    tree.build(triangles, maxDepth);

    auto stop = high_resolution_clock::now();
    
    vector<AABB> voxels;
    tree.collectVoxels(voxels);

    cout << "[Output di path  ] : " << outputPath << endl<< endl;
    writeVoxelOBJ(outputPath, voxels);

    cout << "\n[ STATISTIK ] " << endl;
    cout << "Voxel terbentuk    : " << voxels.size() << endl;
    cout << "Vertex awal        : " << vertices.size() << endl;
    cout << "Faces awal         : " << faces.size() << endl;
    cout << "Kedalaman Octree   : " << maxDepth << endl;
    cout << "Lama waktu         : " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl;
    cout << "Path file .obj     : " << outputPath << endl;

    tree.printStatistics();
    int result = system(command.c_str());
    
    if (result != 0) {
        cerr << "Gagal menjalankan Blender." << endl;
    }
    return 0;
}