#ifndef MISES_AIRFOIL_HPP
#define MISES_AIRFOIL_HPP

#include <MISES/geometry.hpp>
#include <MISES/complex.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <linmath.h>

#include <vector>

namespace mises
{
 
class Airfoil{
  public:
    static void Initialize();
    static void RenderFrame();

    static void CursorPosCallback(GLFWwindow* window, double x_pos, double y_pos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mod);

    static Rect GetFrameRect();
    static void SetFrameRect(Rect frame);

    static double GetFrameRegionWidth();

    static std::vector<Complex> GetCoefficients();
    static void GetVertices(vec2 vertices[]);

    static void CalculateCoefficients();
    static void CalculateVertices();

    static void SaveAirfoil();

  private:
    static void OnVertexUpdate();

    static void DrawGrid();
    static void DrawAxes();
    static void DrawAirfoil();

    static Rect Frame;

    static std::vector<Complex> Coefficients;
    static vec2 Vertices[];
    static double Thickness;
    static double VerticalOffset;
};
    
}


#endif