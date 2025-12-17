#include <MISES/velocities.hpp>

#include <MISES/options.hpp>
#include <MISES/shader.hpp>
#include <MISES/pallet.hpp>
#include <MISES/airfoil.hpp>

using namespace mises;

void Velocities::Initialize(){

}

void Velocities::RenderFrame(){
    Pallet::Create(Frame);

    DrawGrid();
    DrawAxes();
    DrawVelocities();
}

void Velocities::DrawGrid(){
    const double width = Airfoil::GetFrameRegionWidth();
    const double height = Frame.height - 2.0 * MISES_PADDING;

    Shader::SetScale(1.0f);

    const vec2 position = {0.0f, 0.0f};
    Shader::SetTrans(position);

    const vec3 color = MISES_GRID_COLOR;
    Shader::SetColor(color);

    const double x_origin = 0.5 * (Frame.width - width);
    const double y_origin = 0.5 * (Frame.height - height);

    const double x_norm_left  = 0.5 * (1.0 - Frame.width / width);
    const double x_norm_right = 0.5 * (1.0 + Frame.width / width);

    const double y_norm_bottom = 0;
    const double y_norm_top    = MaxVelocity;

    glBegin(GL_LINES);

    for(int i = x_norm_left / MISES_GRID_SPACING; i < x_norm_right / MISES_GRID_SPACING; ++i){ if(i != 0){
        const double x_norm = i * MISES_GRID_SPACING;
        const double x = x_norm * width + x_origin;

        glVertex2f(x, 0.0f);
        glVertex2f(x, Frame.height);
    }}

    for(int i = y_norm_bottom / MISES_GRID_SPACING; i < y_norm_top / MISES_GRID_SPACING; ++i){ if(i != 0){
        const double y_norm = i * MISES_GRID_SPACING;
        const double y = y_norm / MaxVelocity * height + y_origin;
        
        glVertex2f(0.0f,        y);
        glVertex2f(Frame.width, y);
    }}

    glEnd();
}

void Velocities::DrawAxes(){
    const double width = Airfoil::GetFrameRegionWidth();
    const double height = Frame.height - 2.0 * MISES_PADDING;

    Shader::SetScale(1.0f);

    const vec2 position = {0.0f, 0.0f};
    Shader::SetTrans(position);

    const vec3 color = MISES_AXIS_COLOR;
    Shader::SetColor(color);

    const double x_origin = 0.5 * (Frame.width - width);
    const double y_origin = 0.5 * (Frame.height - height);

    glBegin(GL_LINES);

    glVertex2f(0.0f,        y_origin);
    glVertex2f(Frame.width, y_origin);
    glVertex2f(x_origin, 0.0f);
    glVertex2f(x_origin, Frame.height);

    glEnd();
}

void Velocities::DrawVelocities(){
    const double width = Airfoil::GetFrameRegionWidth();
    const double height = Frame.height - 2.0 * MISES_PADDING;

    Shader::SetScale(width);

    const vec2 position = {
        static_cast<float>(0.5f * (Frame.width - width)),
        static_cast<float>(0.5f * (Frame.height - height))
    };
    Shader::SetTrans(position);

    const vec3 color = MISES_AIRFOIL_COLOR;
    Shader::SetColor(color);

    glBegin(GL_LINE_LOOP);

    for(size_t i = 0; i < MISES_VERTEX_COUNT; ++i){
        const float x = VelocityVertices[i][0];
        const float q = VelocityVertices[i][1] * height / width / MaxVelocity;

        glVertex2f(x, q);
    }

    glEnd();
}

