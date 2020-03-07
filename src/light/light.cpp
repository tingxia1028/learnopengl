
#include "light.h"

Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse,
             const glm::vec3 &specular, const LightType lightType)
    : ambient(ambient), diffuse(diffuse), specular(specular),
      lightType(lightType) {}

void Light::draw(ShaderProgram &shaderProgram, std::string lightType,
                 std::string index) {
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].ambient",
                                ambient);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].diffuse",
                                diffuse);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].specular",
                                specular);
}
