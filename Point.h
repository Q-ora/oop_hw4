#ifndef _POINT_H_
#define _POINT_H_

class Point{ // representing a point in 2D, (x, y)
public:
    Point(): x(0), y(0) {};
    Point(double ix, double iy): x(ix), y(iy) {};
    Point(const Point &p): x(p.x), y(p.y) {};
    double getX() const;
    double getY() const;
    bool operator==(const Point &p) const;
private:
    double x;
    double y;
};

#endif