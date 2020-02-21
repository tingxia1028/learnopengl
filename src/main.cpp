#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include "renderengine/DisplayManager.h"
#include "renderengine/loader.h"
#include "renderengine/render.h"
#include "renderengine/shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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
  float vertices[] = {0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
                      -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f};
  float textureCoords[] = {1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  GLuint vertexIndices[] = {0, 1, 3, 1, 2, 3};

  /**
   * load vao
   */
  Data<float> verticesData, textureCoordsData;
  Data<GLuint> indicesData;
  verticesData.data = vertices;
  verticesData.sizeOfData = sizeof(vertices);
  verticesData.dataDimension = 3;
  textureCoordsData.data = textureCoords;
  textureCoordsData.sizeOfData = sizeof(textureCoords);
  textureCoordsData.dataDimension = 2;
  indicesData.data = vertexIndices;
  indicesData.sizeOfData = sizeof(vertexIndices);
  RawModel rawModel = RawModel();
  rawModel.vertexCounts = NumVertices;
  VAOLoader vaoLoader = VAOLoader(verticesData, textureCoordsData, indicesData,
                                  NumVertices, &rawModel);
  vaoLoader.load();

  /**
   * load texture imgs
   */
  TextureLoader textureLoader =
      TextureLoader("../resources/textures/duoduo.JPG", &rawModel);
  textureLoader.load();

  /**
   * transformation
   */
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(-0.2, 0, 0));
  trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
  trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

  /**
   * load shaders
   */
  const int SHADER_NUM = 2;
  ShaderInfo shaders[SHADER_NUM] = {
      {GL_VERTEX_SHADER, "../src/shaders/shader.vs"},
      {GL_FRAGMENT_SHADER, "../src/shaders/shader.fs"}};
  ShaderProgram shaderProgram = ShaderProgram(shaders, SHADER_NUM);
  shaderProgram.createProgram();
  shaderProgram.use();
  shaderProgram.uniformSet1Int("texture1", 0);
  shaderProgram.uniformSetMat4fv("transform", trans);

  /**
   * render loop
   */
  Render render = Render();
  while (!displayManager.shouldClose()) {
    displayManager.processInput();

    render.prepare();
    render.render(*vaoLoader.rawModel);

    displayManager.afterward();
    // poll IO events, eg. mouse moved etc.
    glfwPollEvents();
  }

  vaoLoader.cleanup();
  textureLoader.cleanup();
  shaderProgram.cleanUp();
  displayManager.destroy();
  glfwTerminate();

  return 0;
}