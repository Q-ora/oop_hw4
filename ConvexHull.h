#ifndef _CONVEXHULL_H_
#define _CONVEXHULL_H_

#include "Point.h"
#include "Line.h"
#include "vector"
using namespace std;
class ConvexHull{ // a class for finding convex hull of a set of points. Input is a const pass by reference vector<Point>, output is a pass by reference vector<Point>
public:
    void FindConvexHull_BF(const vector<Point> & input, vector<Point> &output); // find the convex hull of input points
    void FindConvexHull_JM(const vector<Point> & input, vector<Point> &output);
private:
    Line FindLine(const Point &p, const Point &q); // return a directed line pq from point p to q
    bool isOnRight(const Point &p, const Point &q, const Point &r); // return true if r is on the right side of the line from p to q. If pq is horizontal, return true only if r is above it.
    Point FindLowestPoint(const vector<Point> &input); // find the lowest point(the point with the smallest y-coordinate)
    double ComputeAngle(const Point &p, const Point &q, const Point &r); // find the angle pqr of three points
};

#endif