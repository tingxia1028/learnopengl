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
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// render light objects
void renderScene(ShaderProgram &shader, glm::vec3 &lightPos);
void renderCube();

int main() {
  /**
   * window
   */
  Camera camera = Camera(glm::vec3(-0.5f, -0.3f, 0.0f));
  DisplayManager displayManager =
      DisplayManager(SCR_WIDTH, SCR_HEIGHT, "xt screen", &camera);
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
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_MULTISAMPLE);

  /**
   * scene
   */
  std::vector<Model> models;
  Transformation transformation(glm::vec3(0.0f, -0.75f, 0.0f),
                                glm::vec3(0.0002f, 0.0002f, 0.0002f), nullptr);
  Model model("../resources/Sponza-master/sponza.obj", transformation);
  models.push_back(model);

  // lights
  std::vector<Light *> lights;
  glm::vec3 ambientColor(0.2f, 0.2f, 0.2f);
  glm::vec3 diffuseColor(0.9f, 0.9f, 0.9f);
  glm::vec3 specularColor(1.0f, 1.0f, 1.0f);
  //  DirectionalLight directionalLight(ambientColor, diffuseColor,
  //  specularColor,
  //                                    LightType ::DIRECT,
  //                                    glm::vec3(-0.05f, -0.5f, 0.05f));
  //  lights.push_back(&directionalLight);
  //  DirectionalLight directionalLight1(ambientColor, diffuseColor,
  //  specularColor,
  //                                     LightType ::DIRECT,
  //                                     glm::vec3(0.2f, 1.0f, -0.3f));
  //  lights.push_back(&directionalLight1);
  PointLight pointLight(ambientColor, diffuseColor, specularColor,
                        LightType ::POINT, glm::vec3(0.0f, -0.75f, 0.0f), 1.0f,
                        0.09f, 0.032f);
  lights.push_back(&pointLight);

  // skyBox
  std::vector<std::string> skyTexs{
      "../resources/skybox/right.jpg", "../resources/skybox/left.jpg",
      "../resources/skybox/top.jpg",   "../resources/skybox/bottom.jpg",
      "../resources/skybox/front.jpg", "../resources/skybox/back.jpg"};
  SkyBox skyBox(skyTexs);

  Scene scene = Scene(models, &camera, lights, &skyBox);

  /**
   * load shaders
   */
  // model shaders
  std::vector<ShaderInfo> modelShaders{
      {GL_VERTEX_SHADER, "../src/shaders/basic/vertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/basic/fragment.shader"}};
  ShaderProgram modelShader = ShaderProgram(modelShaders);

  // simplest shaders: for outline, light objects, etc
  std::vector<ShaderInfo> simplestShaders{
      {GL_VERTEX_SHADER, "../src/shaders/simplest/simpleVertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/simplest/simpleFrag.shader"}};
  ShaderProgram simplestShader = ShaderProgram(simplestShaders);

  // skyBox shaders
  std::vector<ShaderInfo> skyBoxShaders{
      {GL_VERTEX_SHADER, "../src/shaders/skybox/skyboxVertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/skybox/skyboxFrag.shader"}};
  ShaderProgram skyBoxShader = ShaderProgram(skyBoxShaders);

  // normal shaders
  std::vector<ShaderInfo> normalShaders{
      {GL_VERTEX_SHADER, "../src/shaders/normal/normalVertex.shader"},
      {GL_GEOMETRY_SHADER, "../src/shaders/normal/normalGeometry.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/normal/normalFrag.shader"}};
  ShaderProgram normalShader = ShaderProgram(normalShaders);

  // shadow map shaders
  std::vector<ShaderInfo> directShadowShaders{
      {GL_VERTEX_SHADER, "../src/shaders/shadow/directionLightVertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/shadow/directionLightFrag.shader"}};
  ShaderProgram directShadowShader = ShaderProgram(directShadowShaders);
  std::vector<ShaderInfo> pointShadowShaders{
      {GL_VERTEX_SHADER, "../src/shaders/shadow/pointLightVertex.shader"},
      {GL_GEOMETRY_SHADER, "../src/shaders/shadow/pointLightGeo.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/shadow/pointLightFrag.shader"}};
  ShaderProgram pointShadowShader = ShaderProgram(pointShadowShaders);

  // debug shadow map shaders
  std::vector<ShaderInfo> debugShadowShaders{
      {GL_VERTEX_SHADER, "../src/shaders/shadow/debugDepthVertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/shadow/debugDepthFrag.shader"}};
  ShaderProgram debugShadowShader = ShaderProgram(debugShadowShaders);

  /**
   * render loop
   */
  while (!displayManager.shouldClose()) {

    displayManager.interactionCallback();

    directShadowShader.use();
    Render::prepare(nullptr, displayManager);
    std::set<LightType> directSet;
    directSet.insert(DIRECT);
    Render::renderShadowMap(scene, directShadowShader, directSet);

    pointShadowShader.use();
    Render::prepare(nullptr, displayManager);
    std::set<LightType> pointSet;
    pointSet.insert(POINT);
    pointSet.insert(SPOT);
    Render::renderShadowMap(scene, pointShadowShader, pointSet);

    //    debugShadowShader.use();
    //    Render::prepare(&camera, displayManager);
    //    modelShader.bindUniformBlock("Matrices", 0);
    //    Render::debugRenderShadowMap(scene, debugShadowShader);

    modelShader.use();
    Render::prepare(&camera, displayManager);
    modelShader.bindUniformBlock("Matrices", 0);
    Render::render(scene, modelShader, true, true, true);

    //    normalShader.use();
    //    Render::render(scene, normalShader, false, false);

    //    skyBoxShader.use();
    //    Render::renderSkyBox(scene, skyBoxShader);

    //    simplestShader.use();
    //    for (Light *light : lights) {
    //      renderScene(simplestShader, light->position);
    //    }

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

void renderScene(ShaderProgram &shader, glm::vec3 &lightPos) {
  // cubes
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::mat4(1.0f);
  model = glm::translate(model, lightPos);
  model = glm::scale(model, glm::vec3(0.005f));
  shader.uniformSetMat4("model", model);
  renderCube();
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube() {
  // initialize (if necessary)
  if (cubeVAO == 0) {
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
        // front face
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
        // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // link vertex attributes
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  // render Cube
  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}