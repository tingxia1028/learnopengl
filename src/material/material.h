
#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include <glad/glad.h>
#include <glm/vec3.hpp>
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
  TextureData(){};
  TextureData(const std::string &texturePath, TextureType type);
  GLuint textureID = 0;
};

class Material {
public:
  Material(const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess,
           TextureData &diffuseTex, TextureData &specularTex);
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
  TextureData diffuseTex;
  TextureData specularTex;
  std::vector<TextureData> textures;
};

#endif // OPENGL_MATERIAL_H
