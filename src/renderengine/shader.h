#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

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
  ShaderProgram(std::vector<ShaderInfo> &shaders);
  void use();
  void cleanUp();
  // set uniform values
  void uniformSetVec3F(const std::string name, glm::vec3 value);
  void uniformSetVec4F(const std::string name, glm::vec4 value);
  void uniformSetInt(const std::string name, int value);
  void uniformSetFloat(const std::string name, float value);
  void uniformSetMat4(const std::string name, glm::mat4 &value);
  void uniformSetBool(const std::string name, bool value);
  void bindUniformBlock(const std::string name, int value);

private:
  void checkCompileErrors(unsigned int shader, GLenum type);
  void createProgram();
  std::vector<ShaderInfo> shaders;
};

#endif // OPENGL_SHADER_H
