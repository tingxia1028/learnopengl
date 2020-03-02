
#ifndef OPENGL_DISPLAYMANAGER_H
#define OPENGL_DISPLAYMANAGER_H

#include "../camera/camera.h"
#include <GLFW/glfw3.h>
#include <string>
class DisplayManager {
public:
  DisplayManager(int width, int height, std::string name);
  void init();
  bool shouldClose();
  bool create();
  void afterward();
  void destroy();
  GLFWwindow *getWindow() const;

private:
  GLFWwindow *window;
  int height;
  int width;
  std::string name;
};

#endif // OPENGL_DISPLAYMANAGER_H
