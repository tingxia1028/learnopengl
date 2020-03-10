
#include "render.h"
#include "../light/directionallight.h"
#include "../light/flashlight.h"
#include "../light/pointlight.h"
#include "../light/spotlight.h"
#include "../scene/scene.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Render::prepare(Camera &camera) {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // uniform buffer object
  GLuint uboMatrices;
  glGenBuffers(1, &uboMatrices);
  glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0,
                    2 * sizeof(glm::mat4));
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(camera.getProjectionMatrix()));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(camera.getViewMatrix()));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Render::render(Scene &scene, ShaderProgram &shaderProgram) {

  // camera
  shaderProgram.bindUniformBlock("Matrices", 0);
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

void Render::renderSkyBox(Scene &scene, ShaderProgram &shaderProgram) {

  // camera
  glm::mat4 viewTransform = scene.camera->getViewMatrix();
  shaderProgram.uniformSetMat4("view", viewTransform);
  glm::mat4 projectionTransform = scene.camera->getProjectionMatrix();
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
