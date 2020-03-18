
#include "shader.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

ShaderInfo::ShaderInfo(GLenum sType, std::string fPath) {
  shaderType = sType;
  filePath = fPath;
}

ShaderProgram::ShaderProgram(std::vector<ShaderInfo> &shaders)
    : shaders(shaders) {
  createProgram();
};

void ShaderProgram::createProgram() {
  programID = glCreateProgram();
  for (unsigned int i = 0; i < shaders.size(); ++i) {
    ShaderInfo shaderInfo = shaders[i];

    std::string shaderStr;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      shaderFile.open(shaderInfo.filePath);
      std::stringstream shaderStream;

      shaderStream << shaderFile.rdbuf();
      shaderStream << "\0";
      shaderFile.close();

      shaderStr = shaderStream.str();
    } catch (std::ifstream::failure e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: "
                << shaderInfo.filePath << std::endl;
    }

    GLuint shader = glCreateShader(shaderInfo.shaderType);
    shaderInfo.shaderID = shader;
    const GLchar *shaderCode = shaderStr.c_str();
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderInfo.shaderType);

    glAttachShader(programID, shader);
  }

  std::cout << "shader:" << glGetError() << std::endl;

  glLinkProgram(programID);
  checkCompileErrors(programID, NULL);
}

void ShaderProgram::use() { glUseProgram(programID); }

void ShaderProgram::cleanUp() {
  for (unsigned int i = 0; i < shaders.size(); ++i) {
    glDeleteShader(shaders[i].shaderID);
  }
}

void ShaderProgram::checkCompileErrors(unsigned int shader, GLenum type) {
  GLint success;
  GLchar infoLog[1024];
  if (type) {
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

void ShaderProgram::uniformSetVec3F(const std::string name, glm::vec3 value) {
  glUniform3f(glGetUniformLocation(programID, name.c_str()), value.x, value.y,
              value.z);
}

void ShaderProgram::uniformSetVec4F(const std::string name, glm::vec4 value) {
  glUniform4f(glGetUniformLocation(programID, name.c_str()), value.x, value.y,
              value.z, value.w);
}

void ShaderProgram::uniformSetInt(const std::string name, int value) {
  glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::uniformSetFloat(const std::string name, float value) {
  glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::uniformSetMat4(const std::string name, glm::mat4 &value) {
  glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void ShaderProgram::uniformSetBool(const std::string name, bool value) {
  glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::bindUniformBlock(const std::string name, int value) {
  glUniformBlockBinding(programID,
                        glGetUniformBlockIndex(programID, name.c_str()), value);
}