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

class ShaderProgram {
public:
  GLuint programID;
  ShaderProgram(ShaderInfo *shaders, const int size);
  void createProgram();
  void use();
  void cleanUp();
  // set uniform values
  void uniformSet4Float(const std::string name, float r, float g, float b,
                        float alpha);
  void uniformSet1Int(const std::string name, int value);

private:
  void checkCompileErrors(unsigned int shader, GLenum type);
  ShaderInfo *shaders;
  int size;
};

#endif // OPENGL_SHADER_H
