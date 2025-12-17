#ifndef MISES_VELOCITIES_HPP
#define MISES_VELOCITIES_HPP

#include <MISES/geometry.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace mises{

class Velocities{
  public:
    static void Initialize();
    static void RenderFrame();

    static void CursorPosCallback(GLFWwindow* window, double x_pos, double y_pos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mod);

    static Rect GetFrameRect();
    static void SetFrameRect(Rect frame);

    static void CalculateVelocities();

  private:
    static void DrawGrid();
    static void DrawAxes();
    static void DrawVelocities();

    static Rect Frame;

    static vec2 VelocityVertices[];
    static double MaxVelocity;
};

}

#endif