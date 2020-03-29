
#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include "../renderengine/shader.h"
#include "texture.h"
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>

class Material {
public:
  Material() = default;
  ~Material() = default;
  Material(const glm::vec3 &diffuse, const glm::vec3 &specular, float shininess,
           const std::vector<Texture> &textures);
  void configure(ShaderProgram &shaderProgram, int materialIndex,
                 int textureIndex);
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
  float opacity;
  std::vector<Texture> textures;
  bool hasDiffuseTex = false;
  bool hasSpecularTex = false;
  bool hasNormalMap = false;
  bool hasDepthMap = false;
};

#endif // OPENGL_MATERIAL_H
