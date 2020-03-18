
#ifndef OPENGL_DIRECTIONALLIGHT_H
#define OPENGL_DIRECTIONALLIGHT_H

#include "light.h"
class DirectionalLight : public Light {
public:
  DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                   const glm::vec3 &specular, const LightType lightType,
                   const glm::vec3 &direction);
  void configure(ShaderProgram &shaderProgram, std::string lightType,
                 std::string index) override;
  void configureShadowMatrices(ShaderProgram &shaderProgram) override;
  void activeShadowTex() override;

  glm::vec3 direction;
  glm::mat4 lightSpaceTrans;

private:
  void genShadowMap() override;
};

#endif // OPENGL_DIRECTIONALLIGHT_H
