
#include "directionallight.h"
DirectionalLight::DirectionalLight(const glm::vec3 &ambient,
                                   const glm::vec3 &diffuse,
                                   const glm::vec3 &specular,
                                   const LightType lightType,
                                   const glm::vec3 &direction)
    : Light(ambient, diffuse, specular, lightType), direction(direction) {}

void DirectionalLight::draw(ShaderProgram &shaderProgram, std::string lightType,
                            std::string index) {
  Light::draw(shaderProgram, lightType, index);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].direction",
                                direction);
}
