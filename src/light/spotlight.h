
#ifndef OPENGL_SPOTLIGHT_H
#define OPENGL_SPOTLIGHT_H

#include "light.h"
class SpotLight : public Light {
public:
  glm::vec3 spotPoint;
  float cutoffAngle;
};

#endif // OPENGL_SPOTLIGHT_H
