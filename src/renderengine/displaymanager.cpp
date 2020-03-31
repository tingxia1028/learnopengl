
#include "displaymanager.h"
#include "../utils/fileutils.h"
#include <iostream>

DisplayManager::DisplayManager(int w, int h, std::string name, Camera *camera)
    : width(w), height(h), name(name), camera(camera) {
  lastX = width / 2.0f;
  lastY = height / 2.0f;
};

bool DisplayManager::create() {
  window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);
  return true;
}

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
  glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

bool DisplayManager::shouldClose() { return glfwWindowShouldClose(window); }

void DisplayManager::afterward() {
  // double buffers：front buffer for final result, back buffer for rendering
  // commands
  glfwSwapBuffers(window);
}

void DisplayManager::destroy() { glfwDestroyWindow(window); }

void DisplayManager::interactionCallback() {
  float currentFrame = glfwGetTime();
  float deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  processInput(window, deltaTime);
  windowSizeCallback();
  mouseCallback();
}

void DisplayManager::windowSizeCallback() {
  glfwGetWindowSize(window, &width, &height);
  glViewport(0, 0, 2 * width, 2 * height);
}

void DisplayManager::processInput(GLFWwindow *window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->processKeyboard(CameraMovement::FORWARD, deltaTime);
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->processKeyboard(CameraMovement::BACKWARD, deltaTime);
  else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->processKeyboard(CameraMovement::LEFT, deltaTime);
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->processKeyboard(CameraMovement::RIGHT, deltaTime);
  else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    if (camera->exposure > 0.001f)
      camera->exposure -= 0.001f;
    else
      camera->exposure = 0.0f;
  } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    camera->exposure += 0.001f;
  }
}

void DisplayManager::mouseCallback() {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera->processMouseMovement(xoffset, yoffset);
}