
#include "shader.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>

ShaderInfo::ShaderInfo(GLenum sType, std::string fPath) {
  shaderType = sType;
  filePath = fPath;
}

Shader::Shader(ShaderInfo shaders[], const int size) {
  programID = glCreateProgram();
  for (int i = 0; i < size; ++i) {
    ShaderInfo shaderInfo = shaders[i];

    const char *shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      shaderFile.open(shaderInfo.filePath);
      std::stringstream shaderStream;

      shaderStream << shaderFile.rdbuf();
      shaderFile.close();

      shaderCode = shaderStream.str().c_str();
    } catch (std::ifstream::failure e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    GLuint shader = glCreateShader(shaderInfo.shaderType);
    shaderInfo.shaderID = shader;
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderInfo.shaderType);

    glAttachShader(programID, shader);
  }

  glLinkProgram(programID);

  for (int i = 0; i < size; ++i) {
    glDeleteShader(shaders[i].shaderID);
  }
}

void Shader::use() { glUseProgram(programID); }

void Shader::checkCompileErrors(unsigned int shader, GLenum type) {
  GLint success;
  GLchar infoLog[1024];
  if (NULL != type) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout
          << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout
          << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  }
}

void Shader::set4Float(const std::string name, float r, float g, float b,
                       float alpha) {
  glUniform4f(glGetUniformLocation(programID, name.c_str()), r, g, b, alpha);
}
