
#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include <glm/vec3.hpp>
class material {
public:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

#endif // OPENGL_MATERIAL_H
