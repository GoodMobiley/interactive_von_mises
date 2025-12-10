#ifndef MISES_SHADER_HPP
#define MISES_SHADER_HPP

#include <glad/gl.h>
#include <linmath.h>

namespace mises{

class Shader{
  public:
    static void Initialize();
    static void Select();

    static void SetMVP(const mat4x4 mvp);
    static void SetScale(const float scale);
    static void SetTrans(const vec2 trans);
    static void SetColor(const vec3 color);

  private:
    static GLuint Program;

    static GLint MVPLocation;
    static GLint ScaleLocation;
    static GLint TransLocation;
    static GLint ColorLocation;
};

}

#endif