
#include "transformation.h"
#include <glm/ext/matrix_transform.hpp>

Transformation::Transformation(const glm::vec3 translation,
                               const glm::vec3 scale, Rotate *rotate)
    : translation(translation), scale(scale), rotate(rotate) {}

glm::mat4 Transformation::getTransformationMat() {
  // glm 的实现需要先平移后scale
  glm::mat4 trans = glm::mat4(1.0f);
  if (rotate != NULL) {
    trans = (*rotate).getRotateMat() * trans;
  }
  trans = glm::translate(trans, translation);
  trans = glm::scale(trans, scale);
  return trans;
}
