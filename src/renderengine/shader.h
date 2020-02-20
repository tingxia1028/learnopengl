#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <glad/glad.h>
#include <string>

class ShaderInfo {
public:
  GLenum shaderType;
  std::string filePath;
  GLuint shaderID;
  ShaderInfo(GLenum sType, std::string fPath);
};

class Shader {
public:
  GLuint programID;
  Shader(ShaderInfo shaders[], const int size);
  void use();
  // set uniform values
  void set4Float(const std::string name, float r, float g, float b,
                 float alpha);

private:
  void checkCompileErrors(unsigned int shader, GLenum type);
};

#endif // OPENGL_SHADER_H
