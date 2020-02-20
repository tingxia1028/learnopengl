#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include "renderengine/DisplayManager.h"
#include "renderengine/loader.h"
#include "renderengine/render.h"
#include "renderengine/shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;
constexpr int NumVertices = 6;

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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  /**
   * data
   */
  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f,
                      0.0f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f};
  GLuint vertexIndices[] = {0, 1, 2, 2, 3, 4};

  /**
   * load data
   */
  Loader loader = Loader();
  loader.loadToVAO(vertices, sizeof(vertices), vertexIndices,
                   sizeof(vertexIndices), NumVertices, 3);

  /**
   * load shaders
   */
  const int SHADER_NUM = 2;
  ShaderInfo shaders[SHADER_NUM] = {
      {GL_VERTEX_SHADER,
       "/Users/tingxia/leap/code/c++_code/opengl/src/shaders/shader.vs"},
      {GL_FRAGMENT_SHADER, "/Users/tingxia/leap/code/c++_code/opengl/src/"
                           "shaders/shader.fs"}};
  Shader ourShader(shaders, SHADER_NUM);

  /**
   * render loop
   */
  Render render = Render();
  while (!displayManager.shouldClose()) {
    displayManager.processInput();

    render.prepare();

    ourShader.use();
    ourShader.set4Float("ourColor", 0.0f, sin(glfwGetTime()) / 2.0f + 0.5f,
                        0.0f, 1.0f);
    render.render(loader.rawModel);

    displayManager.afterward();
    // poll IO events, eg. mouse moved etc.
    glfwPollEvents();
  }

  //  loader.cleanup();
  loader.cleanup();
  displayManager.destroy();
  glfwTerminate();

  return 0;
}