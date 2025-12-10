#include <MISES/pallet.hpp>

#include <MISES/options.hpp>
#include <MISES/shader.hpp>

#include <glad/gl.h>

#define USING_MATH_DEFINES
#include <math.h>

using namespace mises;

void Pallet::Create(Rect frame){
    const double radius = MISES_PALLET_RADIUS;
    const size_t corner_vertex_count = 7;

    const double coeff = M_PI_2 / (corner_vertex_count - 1);
    
    Point bevel_centers[] = {
        {frame.width - radius, frame.height - radius},
        {radius, frame.height - radius},
        {radius, radius},
        {frame.width - radius, radius}
    };    

    frame.set_viewport();

    Shader::Select();

    mat4x4 mvp;
    frame.mvp_matrix(mvp);
    Shader::SetMVP(mvp);

    const vec3 color = MISES_PALLET_COLOR;
    Shader::SetColor(color);

    glBegin(GL_POLYGON);

    for(size_t i = 0; i < 4; ++i){
        const double angle_offset = i * M_PI_2;

        for(size_t j = 0; j < corner_vertex_count; ++j){
            glVertex2f(
                bevel_centers[i].x + radius * cos(coeff*j + angle_offset),
                bevel_centers[i].y + radius * sin(coeff*j + angle_offset)
            );
        }
    }

    glEnd();
}