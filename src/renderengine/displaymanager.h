
#ifndef OPENGL_DISPLAYMANAGER_H
#define OPENGL_DISPLAYMANAGER_H

#include <GLFW/glfw3.h>
#include <string>
class DisplayManager {
public:
  DisplayManager(int width, int height, std::string name);
  void init();
  bool create();
  bool shouldClose();
  void processInput();
  void afterward();
  void destroy();

private:
  GLFWwindow *window;
  int height;
  int width;
  std::string name;
  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
};

#endif // OPENGL_DISPLAYMANAGER_H
