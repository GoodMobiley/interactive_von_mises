#include <MISES/user_interface.hpp>

using namespace mises;

GLFWcursor* Movable::hover_cursor(){
    static GLFWcursor* const cursor = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
    return cursor;
}

GLFWcursor* Movable::drag_cursor(){
    return hover_cursor();
}