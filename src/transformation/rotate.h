
#ifndef OPENGL_ROTATE_H
#define OPENGL_ROTATE_H

#include <glm/glm.hpp>
class Rotate {
public:
  Rotate(){};
  virtual glm::mat4 getRotateMat() { return glm::mat4(1.0f); };
};

class EulerRotate : Rotate {
public:
  EulerRotate(float pitch, float yaw, float roll);
  glm::mat4 getRotateMat();

private:
  float pitch;
  float yaw;
  float roll;
};

class ArbitraryAxisRotate : public Rotate {
public:
  ArbitraryAxisRotate(const glm::vec3 &unitAxis, float rotateDegree);
  glm::mat4 getRotateMat();

private:
  const glm::vec3 unitAxis;
  const float rotateDegree;
};

#endif // OPENGL_ROTATE_H
