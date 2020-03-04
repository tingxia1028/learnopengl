
#include "pointlight.h"
PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                       const glm::vec3 &specular, const LightType lightType,
                       const glm::vec3 &position, float constTerm,
                       float linearTerm, float quadraticTerm)
    : Light(ambient, diffuse, specular, lightType), position(position),
      constTerm(constTerm), linearTerm(linearTerm),
      quadraticTerm(quadraticTerm) {}
