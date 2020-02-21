
#include "DisplayManager.h"
#include <GLFW/glfw3.h>
#include <iostream>

DisplayManager::DisplayManager(int w, int h, std::string name)
    : width(w), height(h), name(name){};

void DisplayManager::init() {
  /**
   * window init
   */
  glfwInit();
  // tell GLFW that 3.3 is the OpenGL version we want to use.
  // This way GLFW can make the proper arrangements when creating the OpenGL
  // context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Telling GLFW we want to use the core-profile means we'll get access to a
  // smaller subset of OpenGL features without backwards-compatible features we
  // no longer need
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

bool DisplayManager::create() {
  window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);
  //  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  return true;
}

void DisplayManager::processInput() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
}

void DisplayManager::framebuffer_size_callback(GLFWwindow *window, int width,
                                               int height) {
  //  glViewport(0, 0, width, height);
}

bool DisplayManager::shouldClose() { return glfwWindowShouldClose(window); }

void DisplayManager::afterward() {
  // double buffers：front buffer for final result, back buffer for rendering
  // commands
  glfwSwapBuffers(window);
}

void DisplayManager::destroy() { glfwDestroyWindow(window); }