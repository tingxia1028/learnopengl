
#include "flashlight.h"
FlashLight::FlashLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                       const glm::vec3 &specular, const LightType lightType,
                       float constTerm, float linearTerm, float quadraticTerm,
                       float cutoffCos, float outCutoffCos, Camera *camera)
    : Light(ambient, diffuse, specular, lightType), constTerm(constTerm),
      linearTerm(linearTerm), quadraticTerm(quadraticTerm),
      cutoffCos(cutoffCos), outCutoffCos(outCutoffCos), camera(camera) {}

void FlashLight::draw(ShaderProgram &shaderProgram, std::string lightType,
                      std::string index) {
  Light::draw(shaderProgram, lightType, index);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].position",
                                camera->getPosition());
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].direction",
                                camera->getFront());
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].constant",
                                constTerm);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].linear",
                                linearTerm);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].quadratic",
                                quadraticTerm);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].cutoff",
                                cutoffCos);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].outCutoff",
                                outCutoffCos);
}
