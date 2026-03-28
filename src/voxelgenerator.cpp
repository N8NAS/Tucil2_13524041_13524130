#include <bits/stdc++.h>
#include <cstdlib>
#include "octree.hpp"
#include "renderer.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
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
        cerr << "Usage: " << argv[0] << " <input.obj> [output.obj] [depth]" << endl;
        return 1;
    }

    string inputPath = argv[1];
    string outputPath = (argc > 2) ? argv[2] : "../test/output.obj";
    int maxDepth = (argc > 3) ? atoi(argv[3]) : 5;

    vector<Vertex> vertices;
    vector<Face> faces;

    cout << "[Parsing file obj] : " << inputPath << endl;
    parseOBJ(inputPath, vertices, faces);
    if (vertices.empty()) return 1;

    double minX, minY, minZ, maxX, maxY, maxZ;
    getVoxelMinMax(vertices, minX, minY, minZ, maxX, maxY, maxZ);
    double maxSize = std::max({maxX - minX, maxY - minY, maxZ - minZ});
    vector<Triangle> triangles;
    for (const auto& f : faces) {
        triangles.push_back({{vertices[f.v1-1].x, vertices[f.v1-1].y, vertices[f.v1-1].z},
                             {vertices[f.v2-1].x, vertices[f.v2-1].y, vertices[f.v2-1].z},
                             {vertices[f.v3-1].x, vertices[f.v3-1].y, vertices[f.v3-1].z}});
    }

    auto start = high_resolution_clock::now();
    Octree tree(minX, minY, minZ, maxX, maxY, maxZ);
    tree.build(triangles, maxDepth);
    auto stop = high_resolution_clock::now();
    
    vector<AABB> voxels;
    tree.collectVoxels(voxels);
    writeVoxelOBJ(outputPath, voxels);

    cout << "[Output di path  ] : " << outputPath << endl << endl;
    cout << "\n[ STATISTIK ] " << endl;
    cout << "Voxel terbentuk    : " << voxels.size() << endl;
    cout << "Vertex awal        : " << vertices.size() << endl;
    cout << "Faces awal         : " << faces.size() << endl;
    cout << "Kedalaman Octree   : " << maxDepth << endl;
    cout << "Lama waktu         : " << duration_cast<milliseconds>(stop - start).count() << " ms" << endl;
    cout << "Path file .obj     : " << outputPath << endl;
    tree.printStatistics();

    Display* display = XOpenDisplay(NULL);
    if (!display) return 1;
    
    int width = 1024, height = 768;
    Window window = XCreateSimpleWindow(display, RootWindow(display, DefaultScreen(display)), 0, 0, width, height, 1, 0, 0);
    XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    XMapWindow(display, window);

    std::vector<uint32_t> frameBuffer(width * height);
    SoftwareRenderer renderer(width, height);
    renderer.rotationY = 0.8f;   
    renderer.rotationX = 0.5f;  
    
    renderer.cameraPos.x = (float)((minX + maxX) / 2);
    renderer.cameraPos.y = (float)((minY + maxY) / 2);
    renderer.cameraPos.z = -(maxSize * 5.0f);

    XImage* ximage = XCreateImage(display, DefaultVisual(display, DefaultScreen(display)), DefaultDepth(display, DefaultScreen(display)), 
                                  ZPixmap, 0, (char*)frameBuffer.data(), width, height, 32, 0);
    GC gc = XCreateGC(display, window, 0, NULL);

    bool running = true;
    bool isRightDragging = false;
    int lastX = 0, lastY = 0;

    cout << "\n[ Visualisasi ] : Klik Kanan + Drag untuk rotasi, Scroll untuk Zoom." << endl;

    while (running) {
        renderer.render(voxels, frameBuffer);
        XPutImage(display, window, gc, ximage, 0, 0, 0, 0, width, height);
        
        while (XPending(display)) {
            XEvent event;
            XNextEvent(display, &event);

            if (event.type == KeyPress && XLookupKeysym(&event.xkey, 0) == XK_Escape) running = false;

            if (event.type == ButtonPress) {
                if (event.xbutton.button == 3) {
                    isRightDragging = true;
                    lastX = event.xbutton.x;
                    lastY = event.xbutton.y;
                }
                if (event.xbutton.button == 4) renderer.cameraPos.z += maxSize * 0.2f; 
                if (event.xbutton.button == 5) renderer.cameraPos.z -= maxSize * 0.2f; 
            }

            if (event.type == ButtonRelease && event.xbutton.button == 3) {
                isRightDragging = false;
            }

            if (event.type == MotionNotify && isRightDragging) {
                int dx = event.xmotion.x - lastX;
                int dy = event.xmotion.y - lastY;
                
                renderer.rotationY += dx * 0.005f;
                renderer.rotationX += dy * 0.005f;
                if (renderer.rotationX > 1.5f) renderer.rotationX = 1.5f;
                if (renderer.rotationX < -1.5f) renderer.rotationX = -1.5f;

                lastX = event.xmotion.x;
                lastY = event.xmotion.y;
            }
        }
    }

    XCloseDisplay(display);
    return 0;
}
