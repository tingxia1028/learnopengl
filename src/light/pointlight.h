
#ifndef OPENGL_POINTLIGHT_H
#define OPENGL_POINTLIGHT_H

#include "light.h"
#include <glm/vec3.hpp>
class PointLight : public Light {
public:
  glm::vec3 location;
  float constTerm;
  float linearTerm;
  float quadraticTerm;
};

#endif // OPENGL_POINTLIGHT_H
