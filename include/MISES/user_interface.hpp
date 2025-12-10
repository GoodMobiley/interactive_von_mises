#ifndef MISES_USER_INTERFACE_HPP
#define MISES_USER_INTERFACE_HPP

#include <MISES/geometry.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace mises{

class Movable{
  public:
    virtual Point get_relative_position(Point point) const = 0;
    virtual void  set_position(Point position) = 0;

    virtual GLFWcursor* hover_cursor();
    virtual GLFWcursor* drag_cursor();
};

}

#endif