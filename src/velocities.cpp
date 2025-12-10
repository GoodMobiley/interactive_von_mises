#include <MISES/velocities.hpp>

#include <MISES/complex.hpp>
#include <MISES/airfoil.hpp>

#include <vector>

using namespace mises;

Rect Velocities::Frame;

void Velocities::CursorPosCallback(GLFWwindow* window, double x_pos, double y_pos){

}

void Velocities::MouseButtonCallback(GLFWwindow* window, int button, int action, int mod){

}

Rect Velocities::GetFrameRect(){
    return Frame;
}

void Velocities::SetFrameRect(Rect frame){
    Frame = frame;
}

void Velocities::CalculateVelocities(){
    const std::vector<Complex> coefficients = Airfoil::GetCoefficients();
}