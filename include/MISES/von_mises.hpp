#ifndef MISES_HPP
#define MISES_HPP

#include <MISES/geometry.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace mises{

void Initialize();
void RenderFrame();

void SetFrameRect(Rect render_frame_rect);

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void CursorPosCallaback(GLFWwindow* window, double x_pos, double y_pos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

}

#endif