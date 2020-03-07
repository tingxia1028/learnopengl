
#include "material.h"

Material::Material(const glm::vec3 &diffuse, const glm::vec3 &specular,
                   float shininess, const std::vector<TextureData> &textures)
    : diffuse(diffuse), specular(specular), shininess(shininess),
      textures(textures) {}
