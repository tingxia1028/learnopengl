
#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H

#include "../camera/camera.h"
#include "../renderengine/shader.h"
#include <glm/vec3.hpp>
#include <string>

enum LightType { DIRECT, POINT, SPOT, FLASH };

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
                         std::string index, int depthMapIndex);

  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  LightType lightType;
  // for shadow map
  GLuint shadowMapFBO;
  GLuint depthMapTex;
  glm::mat4 lightSpaceTrans;

private:
  virtual void genShadowMap() = 0;
};

#endif // OPENGL_LIGHT_H
