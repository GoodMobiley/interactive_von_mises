#include <MISES/geometry.hpp>

#include <glad/gl.h>

using namespace mises;

void Rect::set_viewport(){
    glViewport(x, y, width, height);
}

void Rect::mvp_matrix(mat4x4 m){
    double ratio = width / height;
    double scale = 2.0 /  height;

    mat4x4_ortho(m, -ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);
    mat4x4_scale(m, m, scale);  

    m[0][3] = -1.0f;
    m[1][3] = -1.0f;
    m[3][3] = 1.0f;
}

#ifndef MISES_COORDINATES_HPP

double Point::norm() const {
    return std::sqrt(x*x + y*y);
}

bool Point::inside_rect(const Rect rect) const {
    return
        rect.x <= x && (rect.x + rect.width ) >= x
     && rect.y <= y && (rect.y + rect.height) >= y;
}

Point Point::operator+(const Point point) const {
    Point sum;
    sum.x = x + point.x;
    sum.y = y + point.y;

    return sum;
}

Point Point::operator-(const Point point) const {
    return *this + -point;
}

Point Point::operator-() const {
    Point neg;
    neg.x = -x;
    neg.y = -y;

    return neg;
}

#endif