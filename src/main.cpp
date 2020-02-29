#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include "model/Scene.h"
#include "renderengine/displaymanager.h"
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
   * gl global configuration
   */
  //  glEnable(GL_DEPTH_TEST);

  /**
   * data cube
   */
  //  std::vector<float> vertices = {
  //      -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,
  //      0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
  //      0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
  //      -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f,
  //      0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
  //      0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f,
  //      0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,
  //      0.5f,  0.5f,  -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f,
  //      0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,
  //      -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
  //      0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f};
  //  std::vector<float> textureCoords = {
  //      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  //      0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  //      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  //      0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
  //      0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  //      0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  //      0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  std::vector<float> vertices = {0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
                                 -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f};
  std::vector<float> textureCoords = {1.0f, 1.0f, 1.0f, 0.0f,
                                      0.0f, 0.0f, 0.0f, 1.0f};
  std::vector<GLuint> indices = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  TextureData textureData = TextureData("../resources/textures/container2.png");
  int numOfVertices = 6;

  /**
   * load vao
   */
  std::vector<TextureData> textures;
  textures.push_back(textureData);
  std::vector<Model> models;
  Transformation transformation = Transformation(
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), nullptr);
  models.push_back(Model(vertices, numOfVertices, indices, textures,
                         textureCoords, transformation));
  Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  Scene scene = Scene(models, camera);
  VAOLoader vaoLoader = VAOLoader(&scene);
  vaoLoader.load();

  /**
   * load texture
   */
  TextureLoader textureLoader = TextureLoader(&scene);
  textureLoader.load();

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

  /**
   * render loop
   */
  Render render = Render();
  while (!displayManager.shouldClose()) {
    displayManager.processInput();

    render.prepare();
    render.render(scene, shaderProgram);

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
