
#include "render.h"
#include "../light/directionallight.h"
#include "../light/flashlight.h"
#include "../light/pointlight.h"
#include "../light/spotlight.h"
#include "../scene/scene.h"
#include <iostream>

void Render::prepare() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::render(Scene &scene, ShaderProgram &shaderProgram) {

  // camera
  glm::mat4 viewTransform = scene.camera->getViewMatrix();
  shaderProgram.uniformSetMat4("view", viewTransform);
  glm::mat4 projectionTransform = scene.camera->getProjectionMatrix();
  shaderProgram.uniformSetMat4("projection", projectionTransform);
  shaderProgram.uniformSetVec3F("viewPos", scene.camera->getPosition());

  // lights
  int dirNum = 0, pointNum = 0, spotNum = 0;
  std::string lightIndexStr;
  for (unsigned int i = 0; i < scene.lights.size(); ++i) {
    Light *light = scene.lights[i];
    std::string lightTypeStr = LightTypeToString(light->lightType);
    if (light->lightType == LightType::DIRECT) {
      lightIndexStr = std::to_string(dirNum);
      static_cast<DirectionalLight *>(light)->draw(shaderProgram, lightTypeStr,
                                                   lightIndexStr);
      ++dirNum;
    } else if (light->lightType == LightType::POINT) {
      lightIndexStr = std::to_string(pointNum);
      static_cast<PointLight *>(light)->draw(shaderProgram, lightTypeStr,
                                             lightIndexStr);
      ++pointNum;
    } else if (light->lightType == LightType::FLASH) {
      lightIndexStr = std::to_string(spotNum);
      static_cast<FlashLight *>(light)->draw(shaderProgram, lightTypeStr,
                                             lightIndexStr);
      ++spotNum;
    } else if (light->lightType == LightType::SPOT) {
      lightIndexStr = std::to_string(spotNum);
      static_cast<SpotLight *>(light)->draw(shaderProgram, lightTypeStr,
                                            lightIndexStr);
      ++spotNum;
    }
  }
  shaderProgram.uniformSetInt("dirNum", dirNum);
  shaderProgram.uniformSetInt("pointNum", pointNum);
  shaderProgram.uniformSetInt("spotNum", spotNum);

  // models
  for (unsigned int i = 0; i < scene.models.size(); ++i) {
    scene.models[i].draw(shaderProgram);
    std::cout << "render:" << glGetError() << std::endl;
  }
}
