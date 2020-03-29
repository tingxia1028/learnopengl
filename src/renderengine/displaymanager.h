
#ifndef OPENGL_DISPLAYMANAGER_H
#define OPENGL_DISPLAYMANAGER_H

#include "../camera/camera.h"
#include <GLFW/glfw3.h>
#include <string>
class DisplayManager {
public:
  DisplayManager(int width, int height, std::string name, Camera *camera);
  void init();
  bool shouldClose();
  bool create();
  void afterward();
  void destroy();
  void interactionCallback();

  int height;
  int width;

private:
  // interactions
  void windowSizeCallback();
  void processInput(GLFWwindow *window, float deltaTime);
  void mouseCallback();

  GLFWwindow *window;
  Camera *camera;
  std::string name;
  float lastX;
  float lastY;
  bool firstMouse = true;
  float lastFrame;
};

#endif // OPENGL_DISPLAYMANAGER_H
