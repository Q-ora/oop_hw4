#ifndef _LINE_H_
#define _LINE_H_

class Line{ // representing a line in 2D, a*x + b*y + c = 0
public:
    Line(): a(0), b(0), c(0) {};
    Line(double ia, double ib, double ic): a(ia), b(ib), c(ic) {};
    Line(const Line &p): a(p.a), b(p.b), c(p.c) {};
    double getA() const;
    double getB() const;
    double getC() const;
    bool operator==(const Line &p) const;
private:
    double a;
    double b;
    double c;
};

#endif