#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include "light/directionallight.h"
#include "light/flashlight.h"
#include "light/pointlight.h"
#include "light/spotlight.h"
#include "renderengine/displaymanager.h"
#include "renderengine/render.h"
#include "renderengine/shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xoffset, double yoffset);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, float deltaTime);

int main() {
  /**
   * window
   */
  DisplayManager displayManager =
      DisplayManager(SCR_WIDTH, SCR_HEIGHT, "xt screen");
  displayManager.init();
  if (!displayManager.create()) {
    return -1;
  }
  glfwSetFramebufferSizeCallback(displayManager.getWindow(),
                                 framebuffer_size_callback);
  glfwSetCursorPosCallback(displayManager.getWindow(), mouse_callback);
  glfwSetScrollCallback(displayManager.getWindow(), scroll_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  /**
   * gl global configuration
   */
  glEnable(GL_DEPTH_TEST);

  /**
   * scene
   */
  std::vector<Model> models;
  Transformation transformation(glm::vec3(0.0f, -1.75f, 0.0f),
                                glm::vec3(0.2f, 0.2f, 0.2f), nullptr);
  Model model("../resources/nanosuit/nanosuit.obj", transformation);
  models.push_back(model);

  // lights
  std::vector<Light *> lights;
  glm::vec3 ambientColor(0.2f, 0.2f, 0.2f);
  glm::vec3 diffuseColor(0.9f, 0.9f, 0.9f);
  glm::vec3 specularColor(1.0f, 1.0f, 1.0f);
  DirectionalLight directionalLight(ambientColor, diffuseColor, specularColor,
                                    LightType ::DIRECT,
                                    glm::vec3(-0.2f, -1.0f, -0.3f));
  lights.push_back(&directionalLight);
  DirectionalLight directionalLight1(ambientColor, diffuseColor, specularColor,
                                     LightType ::DIRECT,
                                     glm::vec3(0.2f, 1.0f, -0.3f));
  lights.push_back(&directionalLight1);
  PointLight pointLight(ambientColor, diffuseColor, specularColor,
                        LightType ::POINT, glm::vec3(-0.25f, 1.0f, 0.0f), 1.0f,
                        0.09f, 0.032f);
  lights.push_back(&pointLight);
  //  FlashLight flashLight(ambientColor, diffuseColor, specularColor,
  //                        LightType ::FLASH, 1.0f, 0.09f, 0.032f,
  //                        glm::cos(glm::radians(12.5f)),
  //                        glm::cos(glm::radians(17.5f)), &camera);
  //  lights.push_back(&flashLight);

  Scene scene = Scene(models, &camera, lights);

  /**
   * load shaders
   */
  // model shader
  std::vector<ShaderInfo> modelShaders{
      {GL_VERTEX_SHADER, "../src/shaders/vertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/fragment.shader"}};
  ShaderProgram modelShader = ShaderProgram(modelShaders);
  modelShader.createProgram();

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  /**
   * render loop
   */
  Render render = Render();
  while (!displayManager.shouldClose()) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(displayManager.getWindow(), deltaTime);

    render.prepare();

    modelShader.use();
    render.render(scene, modelShader);

    displayManager.afterward();
    // poll IO events, eg. mouse moved etc.
    glfwPollEvents();
  }

  scene.cleanUp();
  modelShader.cleanUp();
  displayManager.destroy();
  glfwTerminate();

  return 0;
}

void processInput(GLFWwindow *window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.processKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
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

  camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.processMouseScroll(yoffset);
}
