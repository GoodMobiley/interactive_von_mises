#include <MISES/pressure.hpp>

#include <MISES/complex.hpp>
#include <MISES/airfoil.hpp>

#include <vector>

using namespace mises;

Rect Pressure::Frame;

void Pressure::CursorPosCallback(GLFWwindow* window, double x_pos, double y_pos){

}

void Pressure::MouseButtonCallback(GLFWwindow* window, int button, int action, int mod){

}

Rect Pressure::GetFrameRect(){
    return Frame;
}

void Pressure::SetFrameRect(Rect frame){
    Frame = frame;
}

void Pressure::CalculatePressure(){
    const std::vector<Complex> coefficients = Airfoil::GetCoefficients();
}