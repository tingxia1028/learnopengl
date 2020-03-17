
#include "camera.h"
#include "../renderengine/displaymanager.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// default camera attribute
const float ANGLEXZ = 0.0f;
const float ANGLEXY = 0.0f;
const float SPEED = 0.5f;
const float SENSITIVITY = 0.2f;
const float ZOOM = 45.0f;
const float NEAR = 0.01f;
const float FAR = 100.0f;

Camera::Camera(const glm::vec3 &position, float width, float height, float near,
               float far)
    : position(position), worldUp(WORLD_UP), width(width), height(height),
      near(near), far(far) {
  updateCameraData();
}

Camera::Camera(const glm::vec3 position)
    : position(position), worldUp(WORLD_UP), angleXZ(ANGLEXZ), angleXY(ANGLEXY),
      movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(ZOOM),
      width(800), height(600), near(NEAR), far(FAR) {
  updateCameraData();
}

void Camera::processKeyboard(CameraMovement movement, float deltaTime) {
  float velocity = deltaTime * movementSpeed;
  switch (movement) {
  case CameraMovement::FORWARD:
    position += front * velocity;
    break;
  case CameraMovement::BACKWARD:
    position -= front * velocity;
    break;
  case CameraMovement ::LEFT:
    position -= right * velocity;
    break;
  case CameraMovement ::RIGHT:
    position += right * velocity;
    break;
  default:;
  }
}

void Camera::processMouseMovement(float xOffset, float yOffset,
                                  bool constrainAngleXY) {
  xOffset *= mouseSensitivity;
  yOffset *= mouseSensitivity;

  angleXZ += xOffset;
  angleXY += yOffset;

  if (constrainAngleXY) {
    angleXY = std::fmaxf(angleXY, -89.0f);
    angleXY = std::fminf(angleXY, 89.0f);
  }
  updateCameraData();
}

void Camera::processMouseScroll(float yOffset) {
  if (fov > 1.0f || fov < 45.0f) {
    fov -= yOffset;
  }
  fov = std::fmaxf(1.0f, fov);
  fov = std::fminf(45.0f, fov);
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(bool isPerspective) {
  return isPerspective
             ? glm::perspective(glm::radians(fov), float(width) / float(height),
                                near, far)
             : glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f,
                          height / 2.0f, near, far);
}

void Camera::updateCameraData() {
  glm::vec3 calculatedFront;
  calculatedFront.x = cos(glm::radians(angleXZ)) * cos(glm::radians(angleXY));
  calculatedFront.y = sin(glm::radians(angleXY));
  calculatedFront.z = sin(glm::radians(angleXZ)) * cos(glm::radians(angleXY));
  front = glm::normalize(calculatedFront);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}
const glm::vec3 &Camera::getPosition() const { return position; }
const glm::vec3 &Camera::getFront() const { return front; }
