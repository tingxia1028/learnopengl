
#ifndef OPENGL_SPOTLIGHT_H
#define OPENGL_SPOTLIGHT_H

#include "pointlight.h"
class SpotLight : public PointLight {
public:
  SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
            const glm::vec3 &specular, const LightType lightType,
            const glm::vec3 &position, float constTerm, float linearTerm,
            float quadraticTerm, const glm::vec3 &direction, float cutoffCos,
            float outCutoffCos);
  void configure(ShaderProgram &shaderProgram, std::string lightType,
                 std::string index, int depthMapIndex) override;

  glm::vec3 direction;
  float cutoffCos;
  float outCutoffCos;
};

#endif // OPENGL_SPOTLIGHT_H
