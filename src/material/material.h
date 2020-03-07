
#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

enum TextureType { DIFFUSE, SPECULAR };

static std::string TexTypeToString(TextureType type) {
  std::string table[] = {"diffuse", "specular"};
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
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
  std::vector<TextureData> textures;
  bool hasDiffuseTex = false;
  bool hasSpecularTex = false;
};

#endif // OPENGL_MATERIAL_H
