
#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// default camera attribute
const float ANGLEXZ = -90.f;
const float ANGLEXY = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
public:
  Camera(const glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
         const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
         const float angleXZ = ANGLEXZ, float angleXY = ANGLEXY,
         float fov = 45.0f, float width = 800.0f, float height = 600.0f,
         float near = 0.1f, float far = 100.0f);
  void processKeyboard(CameraMovement movement, float deltaTime);
  void processMouseMovement(float xOffset, float yOffset,
                            bool constrainAngleXY = true);
  void processMouseScroll(float yOffset);
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();

private:
  // camera outside attributes
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 worldUp;
  glm::vec3 right;
  glm::vec3 up;

  // camera inner attributes
  float fov;
  float width;
  float height;
  float near;
  float far;

  // camera front angles
  float angleXZ;
  float angleXY;

  // camera move options
  float movementSpeed;
  float mouseSensitivity;

  void updateCameraData();
};

#endif // OPENGL_CAMERA_H
