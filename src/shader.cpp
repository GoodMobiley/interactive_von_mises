#include <MISES/shader.hpp>

#include <stdexcept>

using namespace mises;

GLuint Shader::Program = 0;

GLint Shader::MVPLocation;
GLint Shader::ScaleLocation;
GLint Shader::TransLocation;
GLint Shader::ColorLocation;

void Shader::Initialize(){ if(!Program){
    const char *vertex_shader_source = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 mvp;\n"
        "uniform float scale;\n"
        "uniform vec2 trans;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = mvp * (vec4(aPos.x, aPos.y, aPos.z, 1.0) * vec4(scale, scale, scale, 1.0) + vec4(trans.x, trans.y, 0.0, 0.0));\n"
        "}\0";

    const char *fragment_shader_source = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 color;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(color, 1.0f);\n"
        "}\n\0";
    

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLint status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
        throw std::runtime_error("vortex_shader did not compile successfully.");

    
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
        throw std::runtime_error("fragment_shader did not compile successfully.");

    
    Program = glCreateProgram();
    glAttachShader(Program, vertex_shader);
    glAttachShader(Program, fragment_shader);
    glLinkProgram (Program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    MVPLocation   = glGetUniformLocation(Program, "mvp");
    ScaleLocation = glGetUniformLocation(Program, "scale");
    TransLocation = glGetUniformLocation(Program, "trans");
    ColorLocation = glGetUniformLocation(Program, "color");
}}

void Shader::Select(){
    Initialize();
    glUseProgram(Program);

    mat4x4 mvp;
    mat4x4_identity(mvp);
    glUniformMatrix4fv(MVPLocation, 1, GL_TRUE, mvp[0]);

    float scale = 1.0f;
    glUniform1fv(ScaleLocation, 1, &scale);

    vec2 trans = {0.0f, 0.0f};
    glUniform2fv(TransLocation, 1, trans);

    vec3 color = {1.0f, 1.0f, 1.0f};
    glUniform3fv(ColorLocation, 1, color);
}

void Shader::SetMVP(const mat4x4 mvp){
    Initialize();
    glUniformMatrix4fv(MVPLocation, 1, GL_TRUE, mvp[0]);
}

void Shader::SetScale(const float scale){
    Initialize();
    glUniform1fv(ScaleLocation, 1, &scale);
}

void Shader::SetTrans(const vec2 trans){
    Initialize();
    glUniform2fv(TransLocation, 1, trans);
}

void Shader::SetColor(const vec3 color){
    Initialize();
    glUniform3fv(ColorLocation, 1, color);
}