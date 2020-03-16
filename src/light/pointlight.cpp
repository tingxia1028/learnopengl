
#include "pointlight.h"
PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                       const glm::vec3 &specular, const LightType lightType,
                       const glm::vec3 &position, float constTerm,
                       float linearTerm, float quadraticTerm)
    : Light(position, ambient, diffuse, specular, lightType),
      constTerm(constTerm), linearTerm(linearTerm),
      quadraticTerm(quadraticTerm) {}

void PointLight::configure(ShaderProgram &shaderProgram, std::string lightType,
                           std::string index, int depthMapIndex) {
  Light::configure(shaderProgram, lightType, index, depthMapIndex);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].constant",
                                constTerm);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].linear",
                                linearTerm);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].quadratic",
                                quadraticTerm);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].position",
                                position);
}

void PointLight::genShadowMap() {}
