#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include "geometry.hpp"
#include "octree.hpp"

struct Vec2 { float x, y; };
struct Face3D { Point p[4]; float avgZ; uint32_t color; };

class SoftwareRenderer {
public:
    int width, height;
    float rotationY = 0.0f; 
    float rotationX = 0.0f; 
    Point cameraPos = {0, 0, -20};

    SoftwareRenderer(int w, int h) : width(w), height(h) {}

    Vec2 project(Point p) {
        float cosY = cos(rotationY), sinY = sin(rotationY);
        float x1 = p.x * cosY + p.z * sinY;
        float z1 = -p.x * sinY + p.z * cosY;

        float cosX = cos(rotationX), sinX = sin(rotationX);
        float y1 = p.y * cosX - z1 * sinX;
        float z2 = p.y * sinX + z1 * cosX;

        float focalLength = 1000.0f;
        float zFinal = z2 - cameraPos.z;

        if (zFinal < 0.1f) zFinal = 0.1f;

        float screenX = (x1 * focalLength) / zFinal + width / 2.0f;
        float screenY = (-y1 * focalLength) / zFinal + height / 2.0f;

        return { screenX, screenY };
    }

    void drawSolidQuad(vector<uint32_t>& buffer, Vec2 p[4], uint32_t color) {
        int minX = max(0, (int)min({p[0].x, p[1].x, p[2].x, p[3].x}));
        int maxX = min(width - 1, (int)max({p[0].x, p[1].x, p[2].x, p[3].x}));
        int minY = max(0, (int)min({p[0].y, p[1].y, p[2].y, p[3].y}));
        int maxY = min(height - 1, (int)max({p[0].y, p[1].y, p[2].y, p[3].y}));

        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                buffer[y * width + x] = color;
            }
        }
    }

    void render(const vector<AABB>& voxels, vector<uint32_t>& buffer) {
        fill(buffer.begin(), buffer.end(), 0xFF1C1C1C);

        vector<Face3D> faces;
        for (const auto& b : voxels) {
            Point v[8] = {
                {b.min.x, b.min.y, b.min.z}, {b.max.x, b.min.y, b.min.z},
                {b.max.x, b.max.y, b.min.z}, {b.min.x, b.max.y, b.min.z},
                {b.min.x, b.min.y, b.max.z}, {b.max.x, b.min.y, b.max.z},
                {b.max.x, b.max.y, b.max.z}, {b.min.x, b.max.y, b.max.z}
            };
            int fIdx[6][4] = {{0,1,2,3}, {4,5,6,7}, {0,4,7,3}, {1,5,6,2}, {3,2,6,7}, {0,1,5,4}};
            uint32_t shades[6] = {0xFF555555, 0xFF888888, 0xFF666666, 0xFF777777, 0xFF999999, 0xFF444444};

            for (int i = 0; i < 6; i++) {
                Face3D f;
                float sumZ = 0;
                for (int j = 0; j < 4; j++) {
                    f.p[j] = v[fIdx[i][j]];
                    sumZ += f.p[j].z;
                }
                f.avgZ = sumZ / 4.0f;
                f.color = shades[i];
                faces.push_back(f);
            }
        }

        sort(faces.begin(), faces.end(), [](const Face3D& a, const Face3D& b) {
            return a.avgZ > b.avgZ;
        });

        for (auto& f : faces) {
            Vec2 proj[4];
            for (int i = 0; i < 4; i++) proj[i] = project(f.p[i]);
            drawSolidQuad(buffer, proj, f.color);
        }
    }
};