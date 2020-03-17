
#include "render.h"
#include "../light/directionallight.h"
#include "../light/flashlight.h"
#include "../light/pointlight.h"
#include "../light/spotlight.h"
#include "../scene/scene.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

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

void Render::renderShadowMap(Scene &scene, ShaderProgram &shaderProgram) {
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

  for (unsigned int i = 0; i < scene.lights.size(); ++i) {
    glClear(GL_DEPTH_BUFFER_BIT);
    Light *light = scene.lights[i];
    shaderProgram.uniformSetMat4("lightSpaceTrans", light->lightSpaceTrans);
    glm::mat4 result = light->lightSpaceTrans *
                       (scene.models[0].transformation.getTransformationMat());
    glm::mat4 mat = scene.models[0].transformation.getTransformationMat();
    glm::vec4 vec1(10, 75, 104, 1.0f);
    glm::vec4 re1 = mat * vec1;
    glm::vec4 vec2(122, 230, 70, 1.0f);
    glm::vec4 re2 = mat * vec2;
    glm::vec4 result3 = result * glm::vec4(10, 75, 104, 1.0f);
    glm::vec4 result4 = result * glm::vec4(122, 230, 70, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, light->shadowMapFBO);
    render(scene, shaderProgram);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

void Render::debugRenderShadowMap(Scene &scene, ShaderProgram &shaderProgram) {
  Light *light = scene.lights[0];
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, light->depthMapTex);
  shaderProgram.uniformSetFloat("near", 1.0f);
  shaderProgram.uniformSetFloat("far", 7.5f);
  shaderProgram.uniformSetInt("depthMap", 0);
  shaderProgram.uniformSetMat4("lightSpaceTrans", light->lightSpaceTrans);
  render(scene, shaderProgram);
}

void Render::render(Scene &scene, ShaderProgram &shaderProgram, bool withLights,
                    bool withMaterials) {
  // camera
  shaderProgram.uniformSetVec3F("viewPos", scene.camera->getPosition());

  // lights
  if (withLights) {
    configureLights(scene.lights, shaderProgram);
  }

  // models
  for (unsigned int i = 0; i < scene.models.size(); ++i) {
    scene.models[i].draw(shaderProgram, scene.lights.size(), withMaterials);
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
    glActiveTexture(GL_TEXTURE0 + depthMapNum);
    glBindTexture(GL_TEXTURE_2D, light->depthMapTex);
    std::string lightTypeStr = LightTypeToString(light->lightType);
    if (light->lightType == LightType::DIRECT) {
      lightIndexStr = std::to_string(dirNum);
      static_cast<DirectionalLight *>(light)->configure(
          shaderProgram, lightTypeStr, lightIndexStr, depthMapNum);
      ++dirNum;
    } else if (light->lightType == LightType::POINT) {
      lightIndexStr = std::to_string(pointNum);
      static_cast<PointLight *>(light)->configure(shaderProgram, lightTypeStr,
                                                  lightIndexStr, depthMapNum);
      ++pointNum;
    } else if (light->lightType == LightType::FLASH) {
      lightIndexStr = std::to_string(spotNum);
      static_cast<FlashLight *>(light)->configure(shaderProgram, lightTypeStr,
                                                  lightIndexStr, depthMapNum);
      ++spotNum;
    } else if (light->lightType == LightType::SPOT) {
      lightIndexStr = std::to_string(spotNum);
      static_cast<SpotLight *>(light)->configure(shaderProgram, lightTypeStr,
                                                 lightIndexStr, depthMapNum);
      ++spotNum;
    }
    ++depthMapNum;
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
