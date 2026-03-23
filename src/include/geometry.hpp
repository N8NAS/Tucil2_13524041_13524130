#pragma once
#include <cmath>
#include <algorithm>

using namespace std;

struct Vector3 {
    double x, y, z;

    Vector3 operator-(const Vector3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vector3 operator+(const Vector3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    Vector3 operator*(double s) const { return {x * s, y * s, z * s}; }

    Vector3 cross(const Vector3& o) const {
        return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
    }
    double dot(const Vector3& o) const { 
        return x * o.x + y * o.y + z * o.z; 
    }
};

struct Triangle {
    Vector3 v0, v1, v2; 
};

struct AABB {
    Vector3 min, max;
    Vector3 getCenter() const { 
        return {(min.x + max.x) / 2.0, (min.y + max.y) / 2.0, (min.z + max.z) / 2.0}; 
    }
    Vector3 getHalfExtents() const { 
        return {(max.x - min.x) / 2.0, (max.y - min.y) / 2.0, (max.z - min.z) / 2.0}; 
    }
};


inline bool testAxis(const Vector3& axis, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& extents) {
    double p0 = v0.dot(axis);
    double p1 = v1.dot(axis);
    double p2 = v2.dot(axis);

    double r = extents.x * abs(axis.x) + extents.y * abs(axis.y) + extents.z * abs(axis.z);
    double epsilon = 1e-7; 
    r += epsilon;
    double minP = min({p0, p1, p2});
    double maxP = max({p0, p1, p2});
    return !(maxP < -r || minP > r);
}

inline bool checkIntersect(const AABB& box, const Triangle& tri) {
    Vector3 c = box.getCenter();
    Vector3 e = box.getHalfExtents();

    Vector3 v0 = tri.v0 - c;
    Vector3 v1 = tri.v1 - c;
    Vector3 v2 = tri.v2 - c;

    Vector3 f0 = v1 - v0;
    Vector3 f1 = v2 - v1;
    Vector3 f2 = v0 - v2;

    Vector3 axes[9] = {
        {0, -f0.z, f0.y}, {0, -f1.z, f1.y}, {0, -f2.z, f2.y}, 
        {f0.z, 0, -f0.x}, {f1.z, 0, -f1.x}, {f2.z, 0, -f2.x}, 
        {-f0.y, f0.x, 0}, {-f1.y, f1.x, 0}, {-f2.y, f2.x, 0}  
    };
    for (int i = 0; i < 9; i++) {
        if (!testAxis(axes[i], v0, v1, v2, e)) return false; 
    }
    double epsilon = 1e-7;
    if (max({v0.x, v1.x, v2.x}) < -e.x - epsilon || min({v0.x, v1.x, v2.x}) > e.x + epsilon) return false;
    if (max({v0.y, v1.y, v2.y}) < -e.y - epsilon || min({v0.y, v1.y, v2.y}) > e.y + epsilon) return false;
    if (max({v0.z, v1.z, v2.z}) < -e.z - epsilon || min({v0.z, v1.z, v2.z}) > e.z + epsilon) return false;

    Vector3 triNormal = f0.cross(f1); 
    if (!testAxis(triNormal, v0, v1, v2, e)) return false;
    return true; 
}