#include "Line.h"
double Line::getA() const {
    return a;
}
double Line::getB() const{
    return b;
}
double Line::getC() const{
    return c;
}
bool Line::operator==(const Line &p) const{
    return (a == p.a) && (b == p.b) && (c == p.c);
}