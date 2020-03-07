
#ifndef OPENGL_DIRECTIONALLIGHT_H
#define OPENGL_DIRECTIONALLIGHT_H

#include "light.h"
class DirectionalLight : public Light {
public:
  DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                   const glm::vec3 &specular, const LightType lightType,
                   const glm::vec3 &direction);
  void draw(ShaderProgram &shaderProgram, std::string lightType,
            std::string index) override;

  glm::vec3 direction;
};

#endif // OPENGL_DIRECTIONALLIGHT_H
