
#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H

#include <glm/vec3.hpp>
#include <string>

enum LightType { DIRECT, POINT, SPOT, FLASH };

static std::string LightTypeToString(LightType lightType) {
  std::string table[] = {"directLight", "pointLight", "spotLight", "spotLight"};
  return table[lightType];
}

class Light {
public:
  Light(const glm::vec3 &ambient, const glm::vec3 &diffuse,
        const glm::vec3 &specular, const LightType lightType);

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  LightType lightType;
};

#endif // OPENGL_LIGHT_H
