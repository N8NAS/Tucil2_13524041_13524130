#pragma once
#include <bits/stdc++.h>
#include "geometry.hpp"
using namespace std;

#define TopLeftFront 0
#define TopRightFront 1
#define BottomRightFront 2
#define BottomLeftFront 3
#define TopLeftBottom 4
#define TopRightBottom 5
#define BottomRightBack 6
#define BottomLeftBack 7

// Struct Point ANDA TETAP DIPERTAHANKAN
struct Point {
    double x;
    double y;
    double z;
    Point() : x(-1), y(-1), z(-1) {}
    Point(double a, double b, double c) : x(a), y(b), z(c) {}
};

class Octree {
    // Point* point; // INI DIHAPUS karena kita tidak menyimpan 1 titik lagi
    
public:
    bool isLeaf;     // DITAMBAHKAN: Untuk menandai apakah ini sudah jadi Voxel
    int depth;       // DITAMBAHKAN: Untuk mencatat tingkat kedalaman

    // Variabel batas Anda TETAP DIPERTAHANKAN
    Point *topLeftFront, *bottomRightBack;
    vector<Octree*> children;

    // Konstruktor disesuaikan sedikit untuk menerima kedalaman
    Octree(double x1, double y1, double z1, double x2, double y2, double z2, int d = 1) {
        topLeftFront = new Point(x1, y1, z1);
        bottomRightBack = new Point(x2, y2, z2);
        depth = d;
        isLeaf = false;
        children.assign(8, nullptr);
    }

    ~Octree() {
        delete topLeftFront;
        delete bottomRightBack;
        for (auto child : children) if(child != nullptr) delete child;
    }

    void build(const vector<Triangle>& tris, int maxDepth) {
        
        AABB box = {
            {(float)topLeftFront->x, (float)topLeftFront->y, (float)topLeftFront->z},
            {(float)bottomRightBack->x, (float)bottomRightBack->y, (float)bottomRightBack->z}
        };

        vector<Triangle> intersectingTris;
        for (const auto& tri : tris) {
            if (checkIntersect(box, tri)) {
                intersectingTris.push_back(tri);
            }
        }

        if (intersectingTris.empty()) return;

        if (depth == maxDepth) {
            isLeaf = true;
            return;
        }

        double midx = (topLeftFront->x + bottomRightBack->x) / 2.0;
        double midy = (topLeftFront->y + bottomRightBack->y) / 2.0;
        double midz = (topLeftFront->z + bottomRightBack->z) / 2.0;

        children[TopLeftFront] = new Octree(
            topLeftFront->x, topLeftFront->y, topLeftFront->z, 
            midx, midy, midz, depth + 1);

        children[TopRightFront] = new Octree(
            midx, topLeftFront->y, topLeftFront->z, 
            bottomRightBack->x, midy, midz, depth + 1);

        children[BottomRightFront] = new Octree(
            midx, midy, topLeftFront->z, 
            bottomRightBack->x, bottomRightBack->y, midz, depth + 1);

        children[BottomLeftFront] = new Octree(
            topLeftFront->x, midy, topLeftFront->z, 
            midx, bottomRightBack->y, midz, depth + 1);

        children[TopLeftBottom] = new Octree(
            topLeftFront->x, topLeftFront->y, midz, 
            midx, midy, bottomRightBack->z, depth + 1);

        children[TopRightBottom] = new Octree(
            midx, topLeftFront->y, midz, 
            bottomRightBack->x, midy, bottomRightBack->z, depth + 1);

        children[BottomRightBack] = new Octree(
            midx, midy, midz, 
            bottomRightBack->x, bottomRightBack->y, bottomRightBack->z, depth + 1);

        children[BottomLeftBack] = new Octree(
            topLeftFront->x, midy, midz, 
            midx, bottomRightBack->y, bottomRightBack->z, depth + 1);

        for (int i = 0; i < 8; i++) {
            children[i]->build(intersectingTris, maxDepth);
            if (!children[i]->isLeaf && children[i]->children[0] == nullptr) {
                delete children[i];
                children[i] = nullptr;
            }
        }
    }

    void collectVoxels(vector<AABB>& voxels) {
        if (isLeaf) {
            AABB box = {
                {(float)topLeftFront->x, (float)topLeftFront->y, (float)topLeftFront->z},
                {(float)bottomRightBack->x, (float)bottomRightBack->y, (float)bottomRightBack->z}
            };
            voxels.push_back(box);
        } 
        else {
            for (int i = 0; i < 8; i++) {
                if (children[i] != nullptr) {
                    children[i]->collectVoxels(voxels);
                }
            }
        }
    }
};