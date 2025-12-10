#include <MISES/von_mises.hpp>

#include <MISES/options.hpp>
#include <MISES/shader.hpp>
#include <MISES/cylinder.hpp>
#include <MISES/airfoil.hpp>
#include <MISES/pressure.hpp>

using namespace mises;

void mises::Initialize(){
    GLFWwindow* window = glfwGetCurrentContext();

    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    glfwSetCursorPosCallback(window, CursorPosCallaback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);

    Shader::Initialize();
    Cylinder::Initialize();
    Airfoil::Initialize();
    Pressure::Initialize();

    int width;
    int height;

    glfwGetFramebufferSize(window, &width, &height);

    SetFrameRect({
        0.0, 0.0,
        static_cast<double>(width),
        static_cast<double>(height)
    });

    const vec3 background_color = MISES_BACKGROUND_COLOR;

    glClearColor(
        background_color[0],
        background_color[1],
        background_color[2],
        1.0f
    );

    RenderFrame();
}

void mises::RenderFrame(){
    glClear(GL_COLOR_BUFFER_BIT);

    Cylinder::RenderFrame();
    Airfoil::RenderFrame();
    Pressure::RenderFrame();

    glfwSwapBuffers(glfwGetCurrentContext());
}

void mises::SetFrameRect(Rect render_frame_rect){
    Cylinder::SetFrameRect({
        render_frame_rect.x + 0.5 * MISES_MARGIN,
        render_frame_rect.y + 0.5 * MISES_MARGIN,
        0.5 * render_frame_rect.width - MISES_MARGIN,
        render_frame_rect.height - MISES_MARGIN
    });

    Airfoil::SetFrameRect({
        render_frame_rect.x + 0.5 * (render_frame_rect.width + MISES_MARGIN),
        render_frame_rect.y + 0.5 * MISES_MARGIN,
        0.5 * render_frame_rect.width  - MISES_MARGIN,
        0.3 * render_frame_rect.height - MISES_MARGIN
    });

    Pressure::SetFrameRect({
        render_frame_rect.x + 0.5 * (render_frame_rect.width + MISES_MARGIN),
        render_frame_rect.y + 0.3 * render_frame_rect.height + 0.5 * MISES_MARGIN,
        0.5 * render_frame_rect.width  - MISES_MARGIN,
        0.7 * render_frame_rect.height - MISES_MARGIN
    });
}

void mises::FrameBufferSizeCallback(GLFWwindow* window, int width, int height){
    SetFrameRect({
        0.0, 0.0,
        static_cast<double>(width),
        static_cast<double>(height)
    });

    RenderFrame();
}

void mises::CursorPosCallaback(GLFWwindow* window, double x_pos, double y_pos){
    int width;
    int height;

    glfwGetFramebufferSize(window, &width, &height);

    const Point cursor_pos = {
        x_pos,
        height - y_pos
    };

    if(cursor_pos.inside_rect(Cylinder::GetFrameRect())){
        const Rect frame_rect = Cylinder::GetFrameRect();

        Cylinder::CursorPosCallback(
            window, 
            x_pos - frame_rect.x, 
            y_pos + frame_rect.y + frame_rect.height - height
        );
    }
    else if(cursor_pos.inside_rect(Airfoil::GetFrameRect())){
        const Rect frame_rect = Airfoil::GetFrameRect();

        Airfoil::CursorPosCallback(
            window,
            x_pos - frame_rect.x, 
            y_pos + frame_rect.y + frame_rect.height - height
        );
    }
    else{
        glfwSetCursor(window, NULL);
    }
}

void mises::MouseButtonCallback(GLFWwindow* window, int button, int action, int mod){
    double x_pos;
    double y_pos;

    glfwGetCursorPos(window, &x_pos, &y_pos);

    int width;
    int height;

    glfwGetFramebufferSize(window, &width, &height);

    const Point cursor_pos = {
        x_pos,
        height - y_pos
    };

    if(cursor_pos.inside_rect(Cylinder::GetFrameRect())){
        Cylinder::MouseButtonCallback(window, button, action, mod);
    }
    else if(cursor_pos.inside_rect(Airfoil::GetFrameRect())){
        Airfoil::MouseButtonCallback(window, button, action, mod);
    }
}