#include "Point.h"
double Point::getX() const{
    return x;
}
double Point::getY() const{
    return y;
}
bool Point::operator==(const Point &p) const{
    return (x == p.x) && (y == p.y);
}