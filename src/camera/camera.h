
#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H
#define WORLD_UP glm::vec3(0.0f, 1.0f, 0.0f)

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
public:
  Camera() = default;
  ~Camera() = default;
  Camera(const glm::vec3 &position, float width, float height, float near,
         float far);
  Camera(const glm::vec3 position);
  void processKeyboard(CameraMovement movement, float deltaTime);
  void processMouseMovement(float xOffset, float yOffset,
                            bool constrainAngleXY = true);
  void processMouseScroll(float yOffset);
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix(bool isPerspective);
  const glm::vec3 &getPosition() const;
  const glm::vec3 &getFront() const;

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
