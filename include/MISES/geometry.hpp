#ifndef MISES_GEOMETRY_HPP
#define MISES_GEOMETRY_HPP

#include <linmath.h>

#include <complex>

namespace mises{
class Rect{
  public:
    double x;
    double y;
    double width;
    double height;

    void set_viewport();
    void mvp_matrix(mat4x4 m);
};

#ifndef MISES_COORDINATES_HPP

class Point{
  public:
    double x;
    double y;

    double norm() const;
    bool inside_rect(const Rect rect) const;

    Point operator+(const Point point) const;
    Point operator-(const Point point) const;
    Point operator-() const;
};

#endif

}

#endif