
#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H

#include "../camera/camera.h"
#include "../renderengine/shader.h"
#include <glm/vec3.hpp>
#include <string>

enum LightType : int { DIRECT, POINT, SPOT, FLASH };

static std::string LightTypeToString(LightType lightType) {
  std::string table[] = {"directLight", "pointLight", "spotLight", "spotLight"};
  return table[lightType];
}

class Light {
public:
  Light(const glm::vec3 position, const glm::vec3 &ambient,
        const glm::vec3 &diffuse, const glm::vec3 &specular,
        const LightType lightType);
  virtual void configure(ShaderProgram &shaderProgram, std::string lightType,
                         std::string index) = 0;
  virtual void configureShadowMatrices(ShaderProgram &shaderProgram) = 0;
  virtual void activeShadowTex() = 0;

  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  LightType lightType;
  // for shadow map
  GLuint shadowMapFBO;
  GLuint depthMapTex;
  int depthMapIndex = 0;

private:
  virtual void genShadowMap() = 0;
};

#endif // OPENGL_LIGHT_H
