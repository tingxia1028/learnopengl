
#include "rotate.h"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

ArbitraryAxisRotate::ArbitraryAxisRotate(const glm::vec3 &unitAxis, float angle)
    : unitAxis(glm::normalize(unitAxis)), rotateDegree(rotateDegree){};

glm::mat4 ArbitraryAxisRotate::getRotateMat() {
  return glm::rotate(glm::mat4(1.0f), glm::radians(rotateDegree), unitAxis);
}

EulerRotate::EulerRotate(float pitch, float yaw, float rotate)
    : pitch(pitch), yaw(yaw), roll(roll){};

glm::mat4 EulerRotate::getRotateMat() {
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::rotate(trans, glm::radians(pitch), glm::vec3(1, 0, 0));
  trans = glm::rotate(trans, glm::radians(yaw), glm::vec3(0, 1, 0));
  trans = glm::rotate(trans, glm::radians(roll), glm::vec3(0, 0, 1));
  return trans;
}
