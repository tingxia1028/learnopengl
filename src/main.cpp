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

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

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
  //                                    glm::vec3(2.0f, -4.0f, -0.5f));
  //  lights.push_back(&directionalLight);
  //  DirectionalLight directionalLight1(ambientColor, diffuseColor,
  //  specularColor,
  //                                     LightType ::DIRECT,
  //                                     glm::vec3(0.2f, 1.0f, -0.3f));
  //  lights.push_back(&directionalLight1);
  PointLight pointLight(ambientColor, diffuseColor, specularColor,
                        LightType ::POINT, glm::vec3(-0.25f, 1.0f, 0.0f), 1.0f,
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

    //    directShadowShader.use();
    //    std::set<LightType> directSet;
    //    directSet.insert(DIRECT);
    //    Render::renderShadowMap(scene, directShadowShader, directSet);

    pointShadowShader.use();
    std::set<LightType> pointSet;
    pointSet.insert(POINT);
    pointSet.insert(SPOT);
    Render::renderShadowMap(scene, pointShadowShader, pointSet);

    //    Render::prepare(&camera, displayManager);
    //    debugShadowShader.use();
    //    modelShader.bindUniformBlock("Matrices", 0);
    //    Render::debugRenderShadowMap(scene, debugShadowShader);

    //    Render::prepare(&camera, displayManager);
    //    modelShader.use();
    //    modelShader.bindUniformBlock("Matrices", 0);
    //    Render::render(scene, modelShader, true, true, true);

    //    normalShader.use();
    //    Render::render(scene, normalShader, false, false);

    //    skyBoxShader.use();
    //    Render::renderSkyBox(scene, skyBoxShader);

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
