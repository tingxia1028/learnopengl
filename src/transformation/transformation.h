
#ifndef OPENGL_TRANSFORMATION_H
#define OPENGL_TRANSFORMATION_H

#include "rotate.h"
#include <glm/vec3.hpp>
#include <memory>
class Transformation {
public:
  Transformation() = default;
  ~Transformation() = default;
  Transformation(const glm::vec3 translation, const glm::vec3 scale,
                 Rotate *rotate);
  glm::mat4 getTransformationMat();

private:
  glm::vec3 translation;
  glm::vec3 scale;
  Rotate *rotate;
};

#endif // OPENGL_TRANSFORMATION_H
