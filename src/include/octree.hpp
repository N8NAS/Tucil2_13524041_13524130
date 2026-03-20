#pragma once
#include <bits/stdc++.h>
using namespace std;

#define TopLeftFront 0
#define TopRightFront 1
#define BottomRightFront 2
#define BottomLeftFront 3
#define TopLeftBottom 4
#define TopRightBottom 5
#define BottomRightBack 6
#define BottomLeftBack 7

struct Point {
    double x;
    double y;
    double z;
    Point()
        : x(-1), y(-1), z(-1)
    {
    }

    Point(double a, double b, double c)
        : x(a), y(b), z(c)
    {
    }
};
class Octree {
    Point* point;
    Point *topLeftFront, *bottomRightBack;
    vector<Octree*> children;

public:
    Octree()
    {
        point = new Point();
    }

    Octree(double x, double y, double z)
    {
        point = new Point(x, y, z);
    }
    Octree(double x1, double y1, double z1, double x2, double y2, double z2)
    {
        if (x2 < x1
            || y2 < y1
            || z2 < z1) {
            cout << "boundary points are not valid" << endl;
            return;
        }

        point = nullptr;
        topLeftFront
            = new Point(x1, y1, z1);
        bottomRightBack
            = new Point(x2, y2, z2);
        children.assign(8, nullptr);
        for (double i = TopLeftFront;
             i <= BottomLeftBack;
             ++i)
            children[i] = new Octree();
    }
    void insert(double x,
                double y,
                double z)
    {
        if (find(x, y, z)) {
            cout << "Point already exist in the tree" << endl;
            return;
        }
        if (x < topLeftFront->x
            || x > bottomRightBack->x
            || y < topLeftFront->y
            || y > bottomRightBack->y
            || z < topLeftFront->z
            || z > bottomRightBack->z) {
            cout << "Point is out of bound" << endl;
            return;
        }
        double midx = (topLeftFront->x
                    + bottomRightBack->x)
                   / 2;
        double midy = (topLeftFront->y
                    + bottomRightBack->y)
                   / 2;
        double midz = (topLeftFront->z
                    + bottomRightBack->z)
                   / 2;

        double pos = -1;
        if (x <= midx) {
            if (y <= midy) {
                if (z <= midz)
                    pos = TopLeftFront;
                else
                    pos = TopLeftBottom;
            }
            else {
                if (z <= midz)
                    pos = BottomLeftFront;
                else
                    pos = BottomLeftBack;
            }
        }
        else {
            if (y <= midy) {
                if (z <= midz)
                    pos = TopRightFront;
                else
                    pos = TopRightBottom;
            }
            else {
                if (z <= midz)
                    pos = BottomRightFront;
                else
                    pos = BottomRightBack;
            }
        }
        if (children[pos]->point == nullptr) {
            children[pos]->insert(x, y, z);
            return;
        }
        else if (children[pos]->point->x == -1) {
            delete children[pos];
            children[pos] = new Octree(x, y, z);
            return;
        }
        else {
            double x_ = children[pos]->point->x,
                y_ = children[pos]->point->y,
                z_ = children[pos]->point->z;
            delete children[pos];
            children[pos] = nullptr;
            if (pos == TopLeftFront) {
                children[pos] = new Octree(topLeftFront->x,
                                           topLeftFront->y,
                                           topLeftFront->z,
                                           midx,
                                           midy,
                                           midz);
            }

            else if (pos == TopRightFront) {
                children[pos] = new Octree(midx + 1,
                                           topLeftFront->y,
                                           topLeftFront->z,
                                           bottomRightBack->x,
                                           midy,
                                           midz);
            }
            else if (pos == BottomRightFront) {
                children[pos] = new Octree(midx + 1,
                                           midy + 1,
                                           topLeftFront->z,
                                           bottomRightBack->x,
                                           bottomRightBack->y,
                                           midz);
            }
            else if (pos == BottomLeftFront) {
                children[pos] = new Octree(topLeftFront->x,
                                           midy + 1,
                                           topLeftFront->z,
                                           midx,
                                           bottomRightBack->y,
                                           midz);
            }
            else if (pos == TopLeftBottom) {
                children[pos] = new Octree(topLeftFront->x,
                                           topLeftFront->y,
                                           midz + 1,
                                           midx,
                                           midy,
                                           bottomRightBack->z);
            }
            else if (pos == TopRightBottom) {
                children[pos] = new Octree(midx + 1,
                                           topLeftFront->y,
                                           midz + 1,
                                           bottomRightBack->x,
                                           midy,
                                           bottomRightBack->z);
            }
            else if (pos == BottomRightBack) {
                children[pos] = new Octree(midx + 1,
                                           midy + 1,
                                           midz + 1,
                                           bottomRightBack->x,
                                           bottomRightBack->y,
                                           bottomRightBack->z);
            }
            else if (pos == BottomLeftBack) {
                children[pos] = new Octree(topLeftFront->x,
                                           midy + 1,
                                           midz + 1,
                                           midx,
                                           bottomRightBack->y,
                                           bottomRightBack->z);
            }
            children[pos]->insert(x_, y_, z_);
            children[pos]->insert(x, y, z);
        }
    }
    bool find(double x, double y, double z)
    {
        if (x < topLeftFront->x
            || x > bottomRightBack->x
            || y < topLeftFront->y
            || y > bottomRightBack->y
            || z < topLeftFront->z
            || z > bottomRightBack->z)
            return 0;
        double midx = (topLeftFront->x
                    + bottomRightBack->x)
                   / 2;
        double midy = (topLeftFront->y
                    + bottomRightBack->y)
                   / 2;
        double midz = (topLeftFront->z
                    + bottomRightBack->z)
                   / 2;

        double pos = -1;
        if (x <= midx) {
            if (y <= midy) {
                if (z <= midz)
                    pos = TopLeftFront;
                else
                    pos = TopLeftBottom;
            }
            else {
                if (z <= midz)
                    pos = BottomLeftFront;
                else
                    pos = BottomLeftBack;
            }
        }
        else {
            if (y <= midy) {
                if (z <= midz)
                    pos = TopRightFront;
                else
                    pos = TopRightBottom;
            }
            else {
                if (z <= midz)
                    pos = BottomRightFront;
                else
                    pos = BottomRightBack;
            }
        }
        if (children[pos]->point == nullptr) {
            return children[pos]->find(x, y, z);
        }
        else if (children[pos]->point->x == -1) {
            return 0;
        }
        else {
            if (x == children[pos]->point->x
                && y == children[pos]->point->y
                && z == children[pos]->point->z)
                return 1;
        }
        return 0;
    }
};