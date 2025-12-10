#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <MISES/von_mises.hpp>

#include <iostream>

int main(void){

    if(!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Interactive Von Mises Airfoil", NULL, NULL);
    
    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    mises::Initialize();
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}