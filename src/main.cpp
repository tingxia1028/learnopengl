#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include "light/directionallight.h"
#include "renderengine/displaymanager.h"
#include "renderengine/render.h"
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
  //  glEnable(GL_BLEND);
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_MULTISAMPLE);

  /**
   * scene
   */
  std::vector<Model> models;
  Transformation transformation(glm::vec3(0.0f, -0.75f, 0.0f),
                                glm::vec3(0.0005f, 0.0005f, 0.0005f), nullptr);
  Model model("../resources/Sponza-master/sponza.obj", transformation);
  models.push_back(model);

  // lights
  std::vector<Light *> lights;
  glm::vec3 ambientColor(0.2f, 0.2f, 0.2f);
  glm::vec3 diffuseColor(0.5f, 0.5f, 0.5f);
  glm::vec3 specularColor(1.0f, 1.0f, 1.0f);
  DirectionalLight directionalLight(ambientColor, diffuseColor, specularColor,
                                    LightType ::DIRECT,
                                    glm::vec3(-0.05f, -0.5f, 0.05f));
  lights.push_back(&directionalLight);
  //  DirectionalLight directionalLight1(ambientColor, diffuseColor,
  //  specularColor,
  //                                     LightType ::DIRECT,
  //                                     glm::vec3(-0.01f, 0.65f, -0.01f));
  //  lights.push_back(&directionalLight1);
  //  PointLight pointLight(ambientColor, diffuseColor, specularColor,
  //                        LightType ::POINT, glm::vec3(0.0f, -0.6f,
  //                        0.0f), 1.0f, 0.09f, 0.032f);
  //  lights.push_back(&pointLight);

  // skyBox
  std::vector<std::string> skyTexs{
      "../resources/skybox/right.jpg", "../resources/skybox/left.jpg",
      "../resources/skybox/top.jpg",   "../resources/skybox/bottom.jpg",
      "../resources/skybox/front.jpg", "../resources/skybox/back.jpg"};
  SkyBox skyBox(skyTexs);

  Scene scene = Scene(models, &camera, lights, &skyBox);
  std::vector<GBufferTexture> gBufferTexs{
      {GBUFFER_TEXTURE_TYPE_POSITION, "gPosition"},
      {GBUFFER_TEXTRURE_NORMAL, "gNormal"},
      {GBUFFER_TEXTURE_DIFFUSE, "gDiffuse"},
      {GBUFFER_TEXTURE_SPECULAR_SHININESS, "gSpecularShininess"}};
  scene.gBuffer.init(SCR_WIDTH, SCR_HEIGHT, gBufferTexs, true);
  scene.generateFBO(SCR_WIDTH, SCR_HEIGHT);
  scene.generateBlurFBO(SCR_WIDTH, SCR_HEIGHT);

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

  // deferred shaders
  std::vector<ShaderInfo> deferredShaders{
      {GL_VERTEX_SHADER, "../src/shaders/hdr/hdrVertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/hdr/hdrFrag.shader"}};
  ShaderProgram deferredShader = ShaderProgram(deferredShaders);

  // blur shaders
  std::vector<ShaderInfo> blurShaders{
      {GL_VERTEX_SHADER, "../src/shaders/blur/blurVertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/blur/blurFrag.shader"}};
  ShaderProgram blurShader = ShaderProgram(blurShaders);

  // geometry pass: render scene's geometry/color data into gbuffer
  std::vector<ShaderInfo> gbufferShaders{
      {GL_VERTEX_SHADER, "../src/shaders/gbuffer/gbufferVertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/gbuffer/gbufferFrag.shader"}};
  ShaderProgram gbufferShader = ShaderProgram(gbufferShaders);

  // light pass
  std::vector<ShaderInfo> lightpassShaders{
      {GL_VERTEX_SHADER, "../src/shaders/gbuffer/lightpassVertex.shader"},
      {GL_FRAGMENT_SHADER, "../src/shaders/gbuffer/lightpassFrag.shader"}};
  ShaderProgram lightpassShader = ShaderProgram(lightpassShaders);

  /**
   * render loop
   */
  while (!displayManager.shouldClose()) {

    displayManager.interactionCallback();

    // shadow map
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

    // geometry pass
    Render::prepare(&camera, displayManager);
    gbufferShader.use();
    Render::renderGBuffer(gbufferShader, scene);

    // light pass
    lightpassShader.use();
    Render::renderLightPass(lightpassShader, scene);

    // copy geometry's depth buffer to default framebuffer
    scene.gBuffer.bindForRead();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT,
                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // post- processing

    // light cubes
    simplestShader.use();
    for (Light *light : lights) {
      Render::renderLight(simplestShader, light->position, light->diffuse);
    }

    //    skyBoxShader.use();
    //    Render::renderSkyBox(scene, skyBoxShader);

    //    debugShadowShader.use();
    //    Render::prepare(&camera, displayManager);
    //    modelShader.bindUniformBlock("Matrices", 0);
    //    Render::debugRenderShadowMap(scene, debugShadowShader);

    // can use skyBoxShader to draw cube shadow map
    //    skyBoxShader.use();
    //    Render::prepare(&camera, displayManager);
    //    Render::renderSkyBox(scene, skyBoxShader);

    //    glBindFramebuffer(GL_FRAMEBUFFER, scene.deferredFBO);
    //    modelShader.use();
    //    Render::prepare(&camera, displayManager);
    //    modelShader.bindUniformBlock("Matrices", 0);
    //    Render::render(scene, modelShader, true, true, true);
    //
    //    simplestShader.use();
    //    for (Light *light : lights) {
    //      Render::renderLight(simplestShader, light->position,
    //      light->diffuse);
    //    }
    //
    //    skyBoxShader.use();
    //    Render::renderSkyBox(scene, skyBoxShader);
    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //
    //    // blur
    //    blurShader.use();
    //    Render::renderBlur(blurShader, scene);
    //
    //    deferredShader.use();
    //    Render::prepare(nullptr, displayManager);
    //    Render::deferredRender(deferredShader, scene);

    //    normalShader.use();
    //    Render::render(scene, normalShader);

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