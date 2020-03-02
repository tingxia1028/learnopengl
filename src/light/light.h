
#ifndef OPENGL_LIGHT_H
#define OPENGL_LIGHT_H

#include <glm/vec3.hpp>
class Light {
public:
private:
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

#endif // OPENGL_LIGHT_H
