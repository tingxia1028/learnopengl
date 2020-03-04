
#include "render.h"
#include "../light/directionallight.h"
#include "../light/flashlight.h"
#include "../light/pointlight.h"
#include "../light/spotlight.h"
#include "../model/Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Render::prepare() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::render(Scene &scene, ShaderProgram &shaderProgram) {
  glBindVertexArray(scene.vaoID);
  int vaoMaxIndex = scene.maxVaoIndex;
  for (int i = 0; i < vaoMaxIndex; ++i) {
    glEnableVertexAttribArray(i);
  }

  std::vector<Model> models = scene.models;
  int index = 0;
  for (int i = 0; i < models.size(); ++i) {
    Model model = models[i];

    // transformations
    glm::mat4 modelTransform = model.transformation.getTransformationMat();
    shaderProgram.uniformSetMat4("model", modelTransform);
    glm::mat4 viewTransform = scene.camera->getViewMatrix();
    shaderProgram.uniformSetMat4("view", viewTransform);
    glm::mat4 projectionTransform = scene.camera->getProjectionMatrix();
    shaderProgram.uniformSetMat4("projection", projectionTransform);
    shaderProgram.uniformSetVec3F("viewPos", scene.camera->getPosition());

    // lights
    int dirNum = 0, pointNum = 0, spotNum = 0;
    std::string lightIndexStr;
    for (int j = 0; j < scene.lights.size(); ++j) {
      Light *light = scene.lights[j];
      std::string lightTypeStr = LightTypeToString(light->lightType);
      if (light->lightType == LightType::DIRECT) {
        lightIndexStr = std::to_string(dirNum);
        shaderProgram.uniformSetVec3F(lightTypeStr + "s[" + lightIndexStr +
                                          "].direction",
                                      ((DirectionalLight *)light)->direction);
        ++dirNum;
      } else if (light->lightType == LightType::POINT) {
        lightIndexStr = std::to_string(pointNum);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].constant",
                                      ((PointLight *)light)->constTerm);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].linear",
                                      ((PointLight *)light)->linearTerm);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].quadratic",
                                      ((PointLight *)light)->quadraticTerm);
        shaderProgram.uniformSetVec3F(lightTypeStr + "s[" + lightIndexStr +
                                          "].position",
                                      ((PointLight *)light)->position);
        ++pointNum;
      } else if (light->lightType == LightType::FLASH) {
        lightIndexStr = std::to_string(spotNum);
        shaderProgram.uniformSetVec3F(lightTypeStr + "s[" + lightIndexStr +
                                          "].position",
                                      scene.camera->getPosition());
        shaderProgram.uniformSetVec3F(lightTypeStr + "s[" + lightIndexStr +
                                          "].direction",
                                      scene.camera->getFront());
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].constant",
                                      ((FlashLight *)light)->constTerm);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].linear",
                                      ((FlashLight *)light)->linearTerm);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].quadratic",
                                      ((FlashLight *)light)->quadraticTerm);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].cutoff",
                                      ((FlashLight *)light)->cutoffCos);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].outCutoff",
                                      ((FlashLight *)light)->outCutoffCos);
        ++spotNum;
      } else if (light->lightType == LightType::SPOT) {
        lightIndexStr = std::to_string(spotNum);
        shaderProgram.uniformSetVec3F(lightTypeStr + "s[" + lightIndexStr +
                                          "].position",
                                      ((SpotLight *)light)->position);
        shaderProgram.uniformSetVec3F(lightTypeStr + "s[" + lightIndexStr +
                                          "].direction",
                                      ((SpotLight *)light)->direction);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].constant",
                                      ((SpotLight *)light)->constTerm);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].linear",
                                      ((SpotLight *)light)->linearTerm);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].quadratic",
                                      ((SpotLight *)light)->quadraticTerm);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].cutoff",
                                      ((SpotLight *)light)->cutoffCos);
        shaderProgram.uniformSetFloat(lightTypeStr + "s[" + lightIndexStr +
                                          "].outCutoff",
                                      ((SpotLight *)light)->outCutoffCos);
        ++spotNum;
      }

      shaderProgram.uniformSetVec3F(
          lightTypeStr + "s[" + lightIndexStr + "].ambient", light->ambient);
      shaderProgram.uniformSetVec3F(
          lightTypeStr + "s[" + lightIndexStr + "].diffuse", light->diffuse);
      shaderProgram.uniformSetVec3F(
          lightTypeStr + "s[" + lightIndexStr + "].specular", light->specular);
    }
    shaderProgram.uniformSetInt("dirNum", dirNum);
    shaderProgram.uniformSetInt("pointNum", pointNum);
    shaderProgram.uniformSetInt("spotNum", spotNum);

    // materials
    for (int j = 0; j < model.materials.size(); ++j) {
      std::string index = std::to_string(j);
      Material material = model.materials[j];
      shaderProgram.uniformSetFloat("materials[" + index + "].shininess",
                                    material.shininess);
      for (int k = 0; k < material.textures.size(); ++k) {
        TextureData textureData = material.textures[k];
        shaderProgram.uniformSetInt(
            "materials[" + index + "]." + TexTypeToString(textureData.type), k);
        glActiveTexture(GL_TEXTURE0 + k);
        glBindTexture(GL_TEXTURE_2D, textureData.textureID);
      }
    }

    glDrawArrays(GL_TRIANGLES, index, model.numOfVertices);
    index += model.numOfVertices;
    std::cout << "render:" << glGetError() << std::endl;
  }

  for (int i = 0; i < vaoMaxIndex; ++i) {
    glDisableVertexAttribArray(i);
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
