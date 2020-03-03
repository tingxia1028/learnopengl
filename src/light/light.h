
#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H

#include <glm/vec3.hpp>
class Light {
public:
  Light(const glm::vec3 &color, const glm::vec3 &position,
        const glm::vec3 &ambient, const glm::vec3 &diffuse,
        const glm::vec3 &specular);

  glm::vec3 color;
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

#endif // OPENGL_LIGHT_H
