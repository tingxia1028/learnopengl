
#ifndef OPENGL_FLASHLIGHT_H
#define OPENGL_FLASHLIGHT_H

#include "../camera/camera.h"
#include "light.h"
class FlashLight : public Light {
public:
  FlashLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
             const glm::vec3 &specular, const LightType lightType,
             float constTerm, float linearTerm, float quadraticTerm,
             float cutoffCos, float outCutoffCos, Camera *camera);
  void draw(ShaderProgram &shaderProgram, std::string lightType,
            std::string index) override;

  float constTerm;
  float linearTerm;
  float quadraticTerm;
  float cutoffCos;
  float outCutoffCos;
  Camera *camera;
};

#endif // OPENGL_FLASHLIGHT_H
