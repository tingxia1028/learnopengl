
#include "render.h"
#include "../light/directionallight.h"
#include "../light/flashlight.h"
#include "../light/pointlight.h"
#include "../light/spotlight.h"
#include "../scene/scene.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <set>

void Render::prepare(Camera *camera, DisplayManager &displayManager) {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // uniform buffer object
  if (camera) {
    GLuint uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL,
                 GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0,
                      2 * sizeof(glm::mat4));
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                    glm::value_ptr(camera->getProjectionMatrix(true)));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                    glm::value_ptr(camera->getViewMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
  glViewport(0, 0, 2 * displayManager.width, 2 * displayManager.height);
}

void Render::renderShadowMap(Scene &scene, ShaderProgram &shaderProgram,
                             std::set<LightType> &lightTypes) {
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

  for (unsigned int i = 0; i < scene.lights.size(); ++i) {
    Light *light = scene.lights[i];
    if (!lightTypes.count(light->lightType)) {
      continue;
    }
    light->configureShadowMatrices(shaderProgram);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, light->shadowMapFBO);
    render(scene, shaderProgram);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

void Render::debugRenderShadowMap(Scene &scene, ShaderProgram &shaderProgram) {
  Light *light = scene.lights[0];
  light->configureShadowMatrices(shaderProgram);
  shaderProgram.uniformSetFloat("near", 1.0f);
  shaderProgram.uniformSetFloat("far", 7.5f);
  render(scene, shaderProgram, false, false, true);
}

void Render::render(Scene &scene, ShaderProgram &shaderProgram, bool withLights,
                    bool withMaterials, bool withShadowMap) {
  // camera
  shaderProgram.uniformSetVec3F("viewPos", scene.camera->getPosition());

  // lights
  if (withLights) {
    configureLights(scene.lights, shaderProgram);
  }

  // models
  for (unsigned int i = 0; i < scene.models.size(); ++i) {
    std::vector<Light *> nullLights{};
    scene.models[i].draw(shaderProgram,
                         withShadowMap ? scene.lights : nullLights,
                         withMaterials);
    std::cout << "render:" << glGetError() << std::endl;
  }
}

void Render::configureLights(std::vector<Light *> &lights,
                             ShaderProgram &shaderProgram) {
  int dirNum = 0, pointNum = 0, spotNum = 0;
  std::string lightIndexStr;
  int depthMapNum = 0;
  for (unsigned int i = 0; i < lights.size(); ++i) {
    Light *light = lights[i];
    light->depthMapIndex = depthMapNum++;
    std::string lightTypeStr = LightTypeToString(light->lightType);
    switch (light->lightType) {
    case DIRECT:
      lightIndexStr = std::to_string(dirNum++);
      break;
    case POINT:
      lightIndexStr = std::to_string(pointNum++);
      break;
    case FLASH:
    case SPOT:
      lightIndexStr = std::to_string(pointNum++);
      break;
    }
    light->configure(shaderProgram, lightTypeStr, lightIndexStr);
  }
  shaderProgram.uniformSetInt("dirNum", dirNum);
  shaderProgram.uniformSetInt("pointNum", pointNum);
  shaderProgram.uniformSetInt("spotNum", spotNum);
}

void Render::renderSkyBox(Scene &scene, ShaderProgram &shaderProgram) {

  // camera
  glm::mat4 viewTransform = scene.camera->getViewMatrix();
  shaderProgram.uniformSetMat4("view", viewTransform);
  glm::mat4 projectionTransform = scene.camera->getProjectionMatrix(true);
  shaderProgram.uniformSetMat4("projection", projectionTransform);

  glDepthFunc(GL_LEQUAL);
  glBindVertexArray(scene.skyBox->VAO);
  glEnableVertexAttribArray(0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, scene.skyBox->textureID);
  shaderProgram.uniformSetInt("cubemap", 0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glDepthFunc(GL_LESS);
}
