#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include "include/octree.hpp"
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
        cout << "Gagal membuka file: " << path << endl;
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
            string s1, s2, s3;
            ss >> s1 >> s2 >> s3;
            f.v1 = stoi(s1);
            f.v2 = stoi(s2);
            f.v3 = stoi(s3);
            faces.push_back(f);
        }
    }
    file.close();
}
void getVoxelMinMax(vector<Vertex>& vertices, double& minX, double& minY, double& minZ, double& maxX, double& maxY, double& maxZ){
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

vector<AABB> runVoxelization(vector<Face>& faces, vector<Vertex>& vertices, double targetVoxelSize) {
    double minX, minY, minZ, maxX, maxY, maxZ;
    getVoxelMinMax(vertices, minX, minY, minZ, maxX, maxY, maxZ);

    double sizeX = maxX - minX;
    double sizeY = maxY - minY;
    double sizeZ = maxZ - minZ;
    double maxSize = max({sizeX, sizeY, sizeZ});

    int maxDepth = ceil(log2(maxSize / targetVoxelSize)) + 1;
    cout << "Kalkulasi Otomatis - Max Depth: " << maxDepth << endl;

    vector<Triangle> triangles;
    for (const auto& f : faces) {
        Vector3 v0 = {vertices[f.v1 - 1].x, vertices[f.v1 - 1].y, vertices[f.v1 - 1].z};
        Vector3 v1 = {vertices[f.v2 - 1].x, vertices[f.v2 - 1].y, vertices[f.v2 - 1].z};
        Vector3 v2 = {vertices[f.v3 - 1].x, vertices[f.v3 - 1].y, vertices[f.v3 - 1].z};
        triangles.push_back({v0, v1, v2});
    }

    Octree tree(minX, minY, minZ, maxX, maxY, maxZ, 1);
    tree.build(triangles, maxDepth);

    vector<AABB> voxels;
    tree.collectVoxels(voxels);

    return voxels;
}

void writeVoxelOBJ(const string& path, const vector<AABB>& voxels) {
    ofstream file(path);
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
    string inputFile = "../test/teapot.obj"; 
    string outputFile = "output_voxel.obj";

    vector<Vertex> vertices;
    vector<Face> faces;

    cout << "Membaca file " << inputFile << "..." << endl;
    parseOBJ(inputFile, vertices, faces);
    
    if (vertices.empty() || faces.empty()) {
        cout << "File kosong atau tidak ditemukan!" << endl;
        return 1;
    }
    
    auto start = high_resolution_clock::now();
    double ukuranVoxel = 0.8;
    vector<AABB> voxels = runVoxelization(faces, vertices, ukuranVoxel);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Menulis output ke " << outputFile << "..." << endl;
    writeVoxelOBJ(outputFile, voxels);

    cout << "\n--- STATISTIK VOXELIZATION ---" << endl;
    cout << "Banyak Voxel (leaf node)  : " << voxels.size() << " kubus" << endl;
    cout << "Lama Waktu Eksekusi       : " << duration.count() << " milidetik" << endl;
    cout << "Selesai!" << endl;

    return 0;
}