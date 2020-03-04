
#include "light.h"

Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse,
             const glm::vec3 &specular, const LightType lightType)
    : ambient(ambient), diffuse(diffuse), specular(specular),
      lightType(lightType) {}
