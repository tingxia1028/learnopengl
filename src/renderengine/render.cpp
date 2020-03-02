
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
    std::vector<TextureData> textures = models[i].textures;

    // transformations
    glm::mat4 modelTransform = models[i].transformation.getTransformationMat();
    //    shaderProgram.uniformSetMat4("model", modelTransform);
    glm::mat4 viewTransform = scene.camera->getViewMatrix();
    //    shaderProgram.uniformSetMat4("view", viewTransform);
    glm::mat4 projectionTransform = scene.camera->getProjectionMatrix();
    //    shaderProgram.uniformSetMat4("projection", projectionTransform);
    glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
    shaderProgram.uniformSetMat4("model", transform);

    for (int i = 0; i < textures.size(); ++i) {
      shaderProgram.uniformSet1Int("texture" + std::to_string(i), i);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
    }
    glDrawArrays(GL_TRIANGLES, index, models[i].numOfVertices);
    index += models[i].numOfVertices;
    std::cout << "render:" << glGetError() << std::endl;
  }

  for (int i = 0; i < vaoMaxIndex; ++i) {
    glDisableVertexAttribArray(i);
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
