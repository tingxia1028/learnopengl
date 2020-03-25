
#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include "../renderengine/shader.h"
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

enum TextureType { DIFFUSE, SPECULAR, NORMAL, HEIGHT };

static std::string TexTypeToString(TextureType type) {
  std::string table[] = {"diffuse", "specular", "normal", "depth"};
  return table[type];
}

struct TextureData {
  std::string texturePath;
  TextureType type;
  GLuint textureID = 0;
};

class Material {
public:
  Material() = default;
  ~Material() = default;
  Material(const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess,
           const std::vector<TextureData> &textures);
  void configure(ShaderProgram &shaderProgram, int materialIndex,
                 int textureIndex);
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
  float opacity;
  std::vector<TextureData> textures;
  bool hasDiffuseTex = false;
  bool hasSpecularTex = false;
  bool hasNormalMap = false;
  bool hasDepthMap = false;
};

#endif // OPENGL_MATERIAL_H
