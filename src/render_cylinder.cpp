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
    Shader::SetScale(1.0f);

    const vec2 position = {0.0f, 0.0f};
    Shader::SetTrans(position);

    const vec3 color = MISES_GRID_COLOR;
    Shader::SetColor(color);

    const Complex complex_bottom_left = NormalizedPointToComplex(
        FramePointToNormalizedPoint({0.0, 0.0})
    );

    const Complex complex_top_right = NormalizedPointToComplex(
        FramePointToNormalizedPoint({Frame.width, Frame.height})
    );

    const Complex complex_range = complex_top_right - complex_bottom_left;

    const bool draw_xi  = complex_range.real() < Frame.width  * MISES_GRID_SPACING;
    const bool draw_eta = complex_range.imag() < Frame.height * MISES_GRID_SPACING;

    if(draw_xi && draw_eta){
        glBegin(GL_LINES);

        for(
            int i = complex_bottom_left.real() / MISES_GRID_SPACING; 
            i < complex_top_right.real() / MISES_GRID_SPACING; 
            ++i
        ){ if(i != 0){
            const Point position = NormalizedPointToFramePoint(
                ComplexToNormalizedPoint(Complex(i * MISES_GRID_SPACING, 0.0))
            );

            glVertex2f(position.x, 0.0);
            glVertex2f(position.x, Frame.height);
        }}

        for(int i = complex_bottom_left.imag() / MISES_GRID_SPACING; 
            i < complex_top_right.imag() / MISES_GRID_SPACING; 
            ++i
        ){ if(i != 0){
            const Point position = NormalizedPointToFramePoint(
                ComplexToNormalizedPoint(Complex(0.0, i * MISES_GRID_SPACING))
            );

            glVertex2f(0.0,         position.y);
            glVertex2f(Frame.width, position.y);
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
    const Point origin = NormalizedPointToFramePoint(
        ComplexToNormalizedPoint(Complex(0.0, 0.0))
    );

    Shader::SetScale(1.0f);

    const vec2 position = {0.0f, 0.0f};
    Shader::SetTrans(position);

    const vec3 color = MISES_AXIS_COLOR;
    Shader::SetColor(color);

    glBegin(GL_LINES);
    glVertex2f(0.0,         origin.y);
    glVertex2f(Frame.width, origin.y);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(origin.x, 0.0);
    glVertex2f(origin.x, Frame.height);
    glEnd();
}

void Cylinder::DrawCircle(){
    const Point frame_top_right = NormalizedPointToFramePoint({1.0, 1.0});

    Shader::SetScale(frame_top_right.x - 0.5 * Frame.width);

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
    Shader::SetScale(0.5f * MISES_SPRITE_SIZE);

    const vec3 color = MISES_ZERO_COLOR;
    Shader::SetColor(color);

    glBindVertexArray(ZeroVertexArray);

    for(Complex* zero : Zeros){
        const Point position_point = NormalizedPointToFramePoint(
            ComplexToNormalizedPoint(*zero)
        );

        const vec2 position = {
            static_cast<float>(position_point.x),
            static_cast<float>(position_point.y)
        };
        
        Shader::SetTrans(position);

        glDrawArrays(GL_LINE_LOOP, 0, ZeroVertexCount);
    }
}