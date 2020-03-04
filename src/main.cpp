#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include "light/directionallight.h"
#include "light/flashlight.h"
#include "light/pointlight.h"
#include "light/spotlight.h"
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
  std::vector<float> vertices{
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
  std::vector<float> textureCoords{
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  std::vector<float> normals{
      0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
      0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
      0.0f,  0.0f, 1.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
      0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
      0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
      1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
      0.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f};
  std::vector<GLuint> indices;
  int numOfVertices = 36;

  /**
   * scene
   */
  std::vector<Model> models;
  ArbitraryAxisRotate rotate(glm::vec3(1.0f, 1.0f, 1.0f), 45.0f);
  Transformation transformation(glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(0.4f, 0.4f, 0.4f), &rotate);
  std::vector<Material> materials;
  TextureData diffuseTex{"../resources/textures/container_diffuse.png",
                         TextureType::DIFFUSE};
  TextureData specularTex{"../resources/textures/container_specular.png",
                          TextureType::SPECULAR};
  materials.push_back(Material(glm::vec3(1.0f, 0.5f, 0.31f),
                               glm::vec3(0.5f, 0.5f, 0.5f), 64.0f, diffuseTex,
                               specularTex));
  models.push_back(Model(vertices, numOfVertices, indices, textureCoords,
                         transformation, normals, materials));
  Transformation transformation1(glm::vec3(0.75f, 0.75f, 0.0f),
                                 glm::vec3(0.4f, 0.4f, 0.4f), nullptr);
  models.push_back(Model(vertices, numOfVertices, indices, textureCoords,
                         transformation1, normals, materials));
  Transformation transformation2(glm::vec3(-1.0f, 0.75f, -2.0f),
                                 glm::vec3(0.4f, 0.4f, 0.4f), nullptr);
  models.push_back(Model(vertices, numOfVertices, indices, textureCoords,
                         transformation2, normals, materials));

  std::vector<Light *> lights;
  glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
  glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f);
  glm::vec3 ambientColor = diffuseColor * glm::vec3(0.1f);
  glm::vec3 specularColor(1.0f, 1.0f, 1.0f);
  DirectionalLight directionalLight(ambientColor, diffuseColor, specularColor,
                                    LightType ::DIRECT,
                                    glm::vec3(-0.2f, -1.0f, -0.3f));
  lights.push_back(&directionalLight);
  PointLight pointLight(ambientColor, diffuseColor, specularColor,
                        LightType ::POINT, glm::vec3(-0.25f, 1.0f, 0.0f), 1.0f,
                        0.09f, 0.032f);
  lights.push_back(&pointLight);
  SpotLight spotLight(ambientColor, diffuseColor, specularColor,
                      LightType ::SPOT, glm::vec3(-0.75f, 0.0f, 0.0f), 1.0f,
                      0.09f, 0.032f, glm::vec3(1.0f, 0.0f, 0.0f),
                      glm::cos(glm::radians(12.5f)),
                      glm::cos(glm::radians(17.5f)));
  lights.push_back(&spotLight);
  FlashLight flashLight(
      ambientColor, diffuseColor, specularColor, LightType ::FLASH, 1.0f, 0.09f,
      0.032f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
  lights.push_back(&flashLight);
  Scene scene = Scene(models, &camera, lights);

  /**
   * load vao
   */
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
  std::vector<ShaderInfo> modelShaders{
      {GL_VERTEX_SHADER, "../src/shaders/vertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/fragment.shader"}};
  ShaderProgram modelShader = ShaderProgram(modelShaders);
  modelShader.createProgram();

  /**
   * light cube
   */
  std::vector<Model> lightModels;
  for (int i = 0; i < lights.size(); ++i) {
    if (lights[i]->lightType == LightType::DIRECT ||
        lights[i]->lightType == LightType::FLASH) {
      continue;
    }

    Transformation lightTrans = Transformation(
        ((PointLight *)lights[i])->position, glm::vec3(0.2, 0.2, 0.2), nullptr);
    std::vector<TextureData> lightTextures;
    std::vector<float> lightTexCoords;
    std::vector<GLuint> lightIndices;
    std::vector<float> lightNormals;
    std::vector<Material> lightMaterials;
    lightModels.push_back(Model(vertices, numOfVertices, lightIndices,
                                lightTexCoords, lightTrans, lightNormals,
                                lightMaterials));
  }
  std::vector<Light *> lightLights;
  Scene lightScene = Scene(lightModels, &camera, lightLights);
  VAOLoader lightLoader = VAOLoader(&lightScene);
  lightLoader.load();

  // light shaders
  std::vector<ShaderInfo> lightShaders{
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
