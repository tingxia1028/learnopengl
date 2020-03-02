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
   * data cube
   */
  std::vector<float> vertices = {
      -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,
      0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
      0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
      -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f,
      0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
      0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f,
      0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,
      0.5f,  0.5f,  -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f,
      0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,
      -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f};
  std::vector<float> textureCoords = {
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  std::vector<GLuint> indices;
  TextureData textureData = TextureData("../resources/textures/container2.png");
  int numOfVertices = 36;
  std::vector<float> normals;

  /**
   * load vao
   */
  std::vector<TextureData> textures;
  textures.push_back(textureData);
  std::vector<Model> models;
  Transformation transformation = Transformation(
      glm::vec3(-0.75f, -0.75f, 0.0f), glm::vec3(0.4f, 0.4f, 0.4f), nullptr);
  models.push_back(Model(vertices, numOfVertices, indices, textures,
                         textureCoords, transformation, normals));
  Scene scene = Scene(models, &camera);
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
  // model shader
  std::vector<ShaderInfo> modelShaders = {
      {GL_VERTEX_SHADER, "../src/shaders/vertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/fragment.shader"}};
  ShaderProgram modelShader = ShaderProgram(modelShaders);
  modelShader.createProgram();

  /**
   * light cube
   */
  std::vector<Model> lightModels;
  Transformation lightTrans = Transformation(glm::vec3(0.25, 0.25, 0.0f),
                                             glm::vec3(0.2, 0.2, 0.2), nullptr);
  std::vector<TextureData> lightTextures;
  std::vector<float> lightTexCoords;
  std::vector<GLuint> lightIndices;
  std::vector<float> lightNormals;
  lightModels.push_back(Model(vertices, numOfVertices, lightIndices,
                              lightTextures, lightTexCoords, lightTrans,
                              lightNormals));
  Scene lightScene = Scene(lightModels, &camera);
  VAOLoader lightLoader = VAOLoader(&lightScene);
  lightLoader.load();

  // light shaders
  std::vector<ShaderInfo> lightShaders = {
      {GL_VERTEX_SHADER, "../src/shaders/lightVertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/lightFrag.shader"}};
  ShaderProgram lightShader = ShaderProgram(lightShaders);
  lightShader.createProgram();

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  /**
   * render loop
   */
  Render render = Render();
  while (!displayManager.shouldClose()) {

    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(displayManager.getWindow(), deltaTime);

    render.prepare();

    modelShader.use();
    render.render(scene, modelShader);

    lightShader.use();
    render.render(lightScene, lightShader);

    displayManager.afterward();
    // poll IO events, eg. mouse moved etc.
    glfwPollEvents();
  }

  vaoLoader.cleanup();
  lightLoader.cleanup();
  textureLoader.cleanup();
  modelShader.cleanUp();
  lightShader.cleanUp();
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
