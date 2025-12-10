#include <MISES/airfoil.hpp>

#include <MISES/options.hpp>
#include <MISES/cylinder.hpp>
#include <MISES/velocities.hpp>

#include <tinyfiledialogs.h>

#define USING_MATH_DEFINES
#include <math.h>

using namespace mises;

Rect Airfoil::Frame;

std::vector<Complex> Airfoil::Coefficients = {Complex(1.0, 0.0)};
vec2 Airfoil::Vertices[MISES_VERTEX_COUNT] = {{0}};
double Airfoil::Thickness;
double Airfoil::VerticalOffset;

void Airfoil::CursorPosCallback(GLFWwindow* window, double x_pos, double y_pos){

}

void Airfoil::MouseButtonCallback(GLFWwindow* window, int button, int action, int mod){

}

Rect Airfoil::GetFrameRect(){
    return Frame;
}

void Airfoil::SetFrameRect(Rect frame){
    Frame = frame;
}

std::vector<Complex> Airfoil::GetCoefficients(){
    return Coefficients;
}

void Airfoil::CalculateCoefficients(){
    const std::vector<Complex> zeros = Cylinder::GetZeros();
    const size_t count = zeros.size();

    std::vector<Complex> coefficients;
    coefficients.reserve(count - 1);

    for(size_t i = 0; i < count - 1; ++i){
        coefficients.push_back(Complex(0.0, 0.0));
    }

    for(size_t i = 1; i < (0b1<<count); ++i){
        const size_t j = std::popcount(i) - 1;

        if(j > 0){
            Complex product(1.0, 0.0);

            for(size_t k = 0; k < count; ++k){ if(i & (0b1<<k)){
                product *= zeros[k];
            }}

            coefficients[j-1] += product * Complex((j % 2? -1.0: 1.0) / j, 0.0);
        }
    }
    
    Coefficients = coefficients;

    CalculateVertices();
}

void Airfoil::CalculateVertices(){
    const double a    = Cylinder::GetA();
    const Complex mu  = Cylinder::GetMu();
    const double beta = std::atan2(mu.imag(), 1.0 - mu.real());
    const double theta_coeff = 2.0 * M_PI / MISES_VERTEX_COUNT;

    double min_real =  1.0;
    double max_real = -1.0;
    
    double min_imag =  1.0;
    double max_imag = -1.0;

    for(size_t i = 0; i < MISES_VERTEX_COUNT; ++i){
        const Complex zeta = Complex(
            a*cos(theta_coeff*i - beta), 
            a*sin(theta_coeff*i - beta)
        ) + mu;

        Complex z = zeta;

        for(size_t j = 0; j < Coefficients.size(); ++j){
            z += Coefficients[j] / (std::pow(zeta, j+1));
        }

        
        if(z.real() < min_real){
            min_real = z.real();
        }
        else if(z.real() > max_real){
            max_real = z.real();
        }

        
        if(z.imag() < min_imag){
            min_imag = z.imag();
        }
        else if(z.imag() > max_imag){
            max_imag = z.imag();
        }

        Vertices[i][0] = z.real();
        Vertices[i][1] = z.imag();
    }

    const double vertex_coeff = 1.0 / (max_real - min_real);
    const double te_imag = Vertices[0][1];

    for(size_t i = 0; i < MISES_VERTEX_COUNT; ++i){
        Vertices[i][0] = (Vertices[i][0] - min_real) * vertex_coeff;
        Vertices[i][1] = (Vertices[i][1] - te_imag) * vertex_coeff;
    }

    Thickness = (max_imag - min_imag) / (max_real - min_real);
    VerticalOffset = (te_imag - 0.5 * (max_imag + min_imag)) * vertex_coeff;
}

void Airfoil::SaveAirfoil(){
    const char* title = "Test";
    tinyfd_saveFileDialog(title, nullptr, NULL, nullptr, nullptr);
}

void Airfoil::OnVertexUpdate(){
    Velocities::CalculateVelocities();
}