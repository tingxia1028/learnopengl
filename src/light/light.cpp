
#include "light.h"

Light::Light(const glm::vec3 position, const glm::vec3 &ambient,
             const glm::vec3 &diffuse, const glm::vec3 &specular,
             const LightType lightType)
    : position(position), ambient(ambient), diffuse(diffuse),
      specular(specular), lightType(lightType) {}

void Light::configure(ShaderProgram &shaderProgram, std::string lightType,
                      std::string index, int depthMapIndex) {
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].ambient",
                                ambient);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].diffuse",
                                diffuse);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].specular",
                                specular);
  shaderProgram.uniformSetMat4(lightType + "s[" + index + "].lightSpaceTrans",
                               lightSpaceTrans);
  shaderProgram.uniformSetInt(lightType + "s[" + index + "].shadowMap",
                              depthMapIndex);
}
