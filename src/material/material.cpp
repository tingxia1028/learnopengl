
#include "material.h"
Material::Material(const glm::vec3 &diffuse, const glm::vec3 &specular,
                   float shininess, TextureData &diffuseTex,
                   TextureData &specularTex)
    : diffuse(diffuse), specular(specular), shininess(shininess),
      diffuseTex(diffuseTex), specularTex(specularTex) {
  textures.push_back(diffuseTex);
  textures.push_back(specularTex);
}
TextureData::TextureData(const std::string &texturePath, TextureType type)
    : texturePath(texturePath), type(type) {}
