#include <MISES/velocities.hpp>

#include <MISES/options.hpp>
#include <MISES/complex.hpp>
#include <MISES/cylinder.hpp>
#include <MISES/airfoil.hpp>

#include <vector>

using namespace mises;

Rect Velocities::Frame;

vec2 Velocities::VelocityVertices[MISES_VERTEX_COUNT] = {{0}};
double Velocities::MaxVelocity;

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
    const double alpha = 0;

    const double a    = Cylinder::GetA();
    const Complex mu  = Cylinder::GetMu();
    const double beta = std::atan2(mu.imag(), 1.0 - mu.real());
    const double theta_coeff = 2.0 * M_PI / MISES_VERTEX_COUNT;

    vec2 airfoil_vertices[MISES_VERTEX_COUNT];
    Airfoil::GetVertices(airfoil_vertices);
    const std::vector<Complex> coefficients = Airfoil::GetCoefficients();

    Complex d2z_dzeta2_t(0,0);

    for(size_t i = 0; i < coefficients.size(); ++i){
        d2z_dzeta2_t += Complex((i+1)*(i+2),0) * coefficients[i];
    }

    const double q_t = 2.0 * std::abs(std::cos(alpha + beta)) * std::cos(beta) / (std::abs(d2z_dzeta2_t) * (1 - mu.real()));
    MaxVelocity = q_t;

    VelocityVertices[0][0] = VelocityVertices[MISES_VERTEX_COUNT-1][0] = airfoil_vertices[0][0];
    VelocityVertices[0][1] = VelocityVertices[MISES_VERTEX_COUNT-1][1] = q_t;

    for(size_t i = 1; i < MISES_VERTEX_COUNT-1; ++i){
        const double theta = theta_coeff * i;

        const Complex zeta = Complex(
            a*cos(theta - beta), 
            a*sin(theta - beta)
        ) + mu;

        Complex dz_dzeta(1,0);

        for(size_t j = 0; j < coefficients.size(); ++j){
            dz_dzeta -= Complex(j+1,0) * coefficients[j] / std::pow(zeta, j+2);
        }

        const double gamma = 4.0*M_PI * a * std::sin(alpha+beta);

        const Complex w_freestream  =  std::exp(Complex(0, -alpha));
        const Complex w_tangent     = -std::pow(a, 2) * exp(Complex(0, alpha)) / std::pow(zeta-mu, 2);
        const Complex w_circulation =  Complex(0,gamma) / (2.0*M_PI * (zeta-mu));

        const Complex w_cylinder = w_freestream + w_tangent + w_circulation;
        const double q_airfoil  = std::abs(w_cylinder / dz_dzeta);

        if(q_airfoil > MaxVelocity){
            MaxVelocity = q_airfoil;
        }

        VelocityVertices[i][0] = airfoil_vertices[i][0];
        VelocityVertices[i][1] = q_airfoil;
    }
}