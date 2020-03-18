
#include "spotlight.h"

SpotLight::SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                     const glm::vec3 &specular, const LightType lightType,
                     const glm::vec3 &position, float constTerm,
                     float linearTerm, float quadraticTerm,
                     const glm::vec3 &direction, float cutoffCos,
                     float outCutoffCos)
    : PointLight(ambient, diffuse, specular, lightType, position, constTerm,
                 linearTerm, quadraticTerm),
      direction(direction), cutoffCos(cutoffCos), outCutoffCos(outCutoffCos) {}

void SpotLight::configure(ShaderProgram &shaderProgram, std::string lightType,
                          std::string index) {
  PointLight::configure(shaderProgram, lightType, index);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].direction",
                                direction);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].cutoff",
                                cutoffCos);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].outCutoff",
                                outCutoffCos);
}
