
#include "directionallight.h"
DirectionalLight::DirectionalLight(const glm::vec3 &ambient,
                                   const glm::vec3 &diffuse,
                                   const glm::vec3 &specular,
                                   const LightType lightType,
                                   const glm::vec3 &direction)
    : Light(ambient, diffuse, specular, lightType), direction(direction) {}
