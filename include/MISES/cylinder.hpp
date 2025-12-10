#ifndef MISES_CYLINDER_HPP
#define MISES_CYLINDER_HPP

#include <MISES/complex.hpp>
#include <MISES/geometry.hpp>
#include <MISES/user_interface.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace mises
{

class Cylinder{
  public:
    static void Initialize();
    static void RenderFrame();
    
    static void CursorPosCallback(GLFWwindow* window, double x_pos, double y_pos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mod);

    static Rect GetFrameRect();
    static void SetFrameRect(Rect frame);

    static double  GetA();
    static Complex GetMu();

    static bool SetMu(const Complex mu);

    static std::vector<Complex> GetZeros();
    static bool AddZero(const Complex value);
    static bool RemoveZero(Complex* const zero);
    static bool SetZero(Complex* const zero, const Complex value);
    static void CorrectZeros();

  private:
    static void UpdateSelection();
    static void UpdateCursor();

    static void OnZeroUpdate();
    static void OnMuUpdate();

    static void DrawGrid();
    static void DrawAxes();
    static void DrawCircle();
    static void DrawZeros();

    static Rect Frame;

    static Movable* SelectedItem;
    static Point CursorPos;
    static Point RelativeSelectionPos;
    static bool Mouse1Hold;

    static double A;
    static Complex Mu;

    static std::vector<Complex*> Zeros;

    static const size_t CircleVertexCount;
    static GLuint CircleVertexArray;

    static const size_t CircleCenterVertexCount;
    static GLuint CircleCenterVertexArray;

    static const size_t ZeroVertexCount;
    static GLuint ZeroVertexArray;
};
    
}

#endif