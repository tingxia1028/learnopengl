
#include "light.h"

Light::Light(const glm::vec3 &color, const glm::vec3 &position,
             const glm::vec3 &ambient, const glm::vec3 &diffuse,
             const glm::vec3 &specular)
    : color(color), position(position), ambient(ambient), diffuse(diffuse),
      specular(specular) {}
