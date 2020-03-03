
#include "render.h"
#include "../model/Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Render::prepare() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0);
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
    for (int j = 0; j < scene.lights.size(); ++j) {
      std::string index = std::to_string(j);
      Light light = scene.lights[j];
      shaderProgram.uniformSetVec3F("lights[" + index + "].position",
                                    light.position);
      shaderProgram.uniformSetVec3F("lights[" + index + "].ambient",
                                    light.ambient);
      shaderProgram.uniformSetVec3F("lights[" + index + "].diffuse",
                                    light.diffuse);
      shaderProgram.uniformSetVec3F("lights[" + index + "].specular",
                                    light.specular);
    }

    // materials
    for (int j = 0; j < model.materials.size(); ++j) {
      std::string index = std::to_string(j);
      Material material = model.materials[j];
      shaderProgram.uniformSetVec3F("material.specular", material.specular);
      shaderProgram.uniformSetFloat("material.shininess", material.shininess);
      for (int k = 0; k < material.textures.size(); ++k) {
        TextureData textureData = material.textures[k];
        shaderProgram.uniformSetInt(
            "material." + TexTypeToString(textureData.type), k);
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
