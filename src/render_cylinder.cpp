#include <MISES/cylinder.hpp>

#include <MISES/options.hpp>
#include <MISES/shader.hpp>
#include <MISES/pallet.hpp>

#define USING_MATH_DEFINES
#include <math.h>

using namespace mises;

const size_t Cylinder::CircleVertexCount = MISES_VERTEX_COUNT;
GLuint Cylinder::CircleVertexArray = 0;

const size_t Cylinder::CircleCenterVertexCount = 4;
GLuint Cylinder::CircleCenterVertexArray = 0;

const size_t Cylinder::ZeroVertexCount = 36;
GLuint Cylinder::ZeroVertexArray = 0;

void Cylinder::Initialize(){
    Shader::Initialize();

    if(!CircleVertexArray){
        glGenVertexArrays(1, &CircleVertexArray);
        glBindVertexArray(CircleVertexArray);

        const double coeff = 2.0 * M_PI / CircleVertexCount;
        vec2 circle_verticies[CircleVertexCount];

        for(size_t i = 0; i < CircleVertexCount; ++i){
            circle_verticies[i][0] = std::cos(coeff*i);
            circle_verticies[i][1] = std::sin(coeff*i);
        }

        GLuint circle_buffer;

        glGenBuffers(1, &circle_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, circle_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circle_verticies), circle_verticies, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
        glEnableVertexAttribArray(0);
    }
    if(!CircleCenterVertexArray){
        glGenVertexArrays(1, &CircleCenterVertexArray);
        glBindVertexArray(CircleCenterVertexArray);

        vec2 circle_center_verticies[CircleCenterVertexCount] = {
            {-1.0,  0.0},
            { 1.0,  0.0},
            { 0.0, -1.0},
            { 0.0,  1.0}
        };

        GLuint circle_center_buffer;

        glGenBuffers(1, &circle_center_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, circle_center_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circle_center_verticies), circle_center_verticies, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
        glEnableVertexAttribArray(0);
    }
    if(!ZeroVertexArray){
        glGenVertexArrays(1, &ZeroVertexArray);
        glBindVertexArray(ZeroVertexArray);

        const double coeff = 2.0 * M_PI / ZeroVertexCount;
        vec2 zero_verticies[ZeroVertexCount];

        for(size_t i = 0; i < ZeroVertexCount; ++i){
            zero_verticies[i][0] = std::cos(coeff*i);
            zero_verticies[i][1] = std::sin(coeff*i);
        }

        GLuint zero_buffer;

        glGenBuffers(1, &zero_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, zero_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(zero_verticies), zero_verticies, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
        glEnableVertexAttribArray(0);
    }
}

void Cylinder::RenderFrame(){
    Initialize();

    Pallet::Create(Frame);

    DrawGrid();
    DrawAxes();
    DrawCircle();
    DrawZeros();
}

void Cylinder::DrawGrid(){
    const double min_dim = Frame.width > Frame.height ? Frame.height : Frame.width;
    const double coeff = 0.5 * min_dim - MISES_PADDING;

    Shader::SetScale(1.0f);

    const vec2 position = {0.0f, 0.0f};
    Shader::SetTrans(position);

    const vec3 color = MISES_GRID_COLOR;
    Shader::SetColor(color);

    const double x_norm_left  = -0.5 * Frame.width / coeff;
    const double x_norm_right = -x_norm_left;

    const double y_norm_bottom = -0.5 * Frame.height / coeff;
    const double y_norm_top    = -y_norm_bottom;

    double xi_left  = x_norm_left  * A + Mu.real();
    double xi_right = x_norm_right * A + Mu.real();

    double eta_bottom = y_norm_bottom * A + Mu.imag();
    double eta_top    = y_norm_top    * A + Mu.imag();

    const bool draw_xi  = xi_right - xi_left    < Frame.width  * MISES_GRID_SPACING;
    const bool draw_eta = eta_top  - eta_bottom < Frame.height * MISES_GRID_SPACING;

    if(draw_xi && draw_eta){
        glBegin(GL_LINES);

        for(int i = xi_left / MISES_GRID_SPACING; i < xi_right / MISES_GRID_SPACING; ++i){ if(i != 0){
            const double xi = i * MISES_GRID_SPACING;
            const double x_norm = (xi - Mu.real()) / A;
            const double x = x_norm * coeff + 0.5 * Frame.width;

            glVertex2f(x, 0.0);
            glVertex2f(x, Frame.height);
        }}

        for(int i = eta_bottom / MISES_GRID_SPACING; i < eta_top / MISES_GRID_SPACING; ++i){ if(i != 0){
            const double eta = i * MISES_GRID_SPACING;
            const double y_norm = (eta - Mu.imag()) / A;
            const double y = y_norm * coeff + 0.5 * Frame.height;

            glVertex2f(0.0, y);
            glVertex2f(Frame.width, y);
        }}

        glEnd();
    }
    else{
        glBegin(GL_POLYGON);

        glVertex2f(0.0f,        0.0f        );
        glVertex2f(0.0f,        Frame.height);
        glVertex2f(Frame.width, Frame.height);
        glVertex2f(Frame.width, 0.0f        );

        glEnd();
    }    
}

void Cylinder::DrawAxes(){
    const double min_dim = Frame.width > Frame.height ? Frame.height : Frame.width;
    const double coeff = 0.5 * min_dim - MISES_PADDING;

    const double y_norm = -Mu.imag() / A;
    const double y = 0.5 * Frame.height + y_norm * coeff;

    const double x_norm = -Mu.real() / A;
    const double x = 0.5 * Frame.width + x_norm * coeff;

    Shader::SetScale(1.0f);

    const vec2 position = {0.0f, 0.0f};
    Shader::SetTrans(position);

    const vec3 color = MISES_AXIS_COLOR;
    Shader::SetColor(color);

    glBegin(GL_LINES);
    glVertex2f(0.0, y);
    glVertex2f(Frame.width, y);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x, 0.0);
    glVertex2f(x, Frame.height);
    glEnd();
}

void Cylinder::DrawCircle(){
    const double min_dim = Frame.width > Frame.height ? Frame.height : Frame.width;

    Shader::SetScale(0.5 * min_dim - MISES_PADDING);

    const vec2 position = {
        static_cast<float>(0.5f * Frame.width), 
        static_cast<float>(0.5f * Frame.height)
    };
    Shader::SetTrans(position);

    const vec3 circle_color = MISES_CIRCLE_COLOR;
    Shader::SetColor(circle_color);

    glBindVertexArray(CircleVertexArray);
    glDrawArrays(GL_LINE_LOOP, 0, CircleVertexCount);

    Shader::SetScale(0.5f * MISES_SPRITE_SIZE);

    const vec3 circle_center_color = MISES_CIRCLE_CENTER_COLOR;
    Shader::SetColor(circle_center_color);

    glBindVertexArray(CircleCenterVertexArray);
    glDrawArrays(GL_LINES, 0, CircleCenterVertexCount);
}

void Cylinder::DrawZeros(){
    const double min_dim = Frame.width > Frame.height ? Frame.height : Frame.width;

    Shader::SetScale(0.5f * MISES_SPRITE_SIZE);

    const vec3 color = MISES_ZERO_COLOR;
    Shader::SetColor(color);

    glBindVertexArray(ZeroVertexArray);

    for(Complex* zero : Zeros){
        const double coeff = 0.5 * min_dim - MISES_PADDING;

        const double x_norm = (zero->real() - Mu.real()) / A;
        const double y_norm = (zero->imag() - Mu.imag()) / A;

        const vec2 position = {
            static_cast<float>(0.5f * Frame.width  + x_norm * coeff),
            static_cast<float>(0.5f * Frame.height + y_norm * coeff)
        };
        Shader::SetTrans(position);

        glDrawArrays(GL_LINE_LOOP, 0, ZeroVertexCount);
    }
}