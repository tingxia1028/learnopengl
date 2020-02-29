
#include "render.h"
#include "../model/Scene.h"

void Render::prepare() {
  glClearColor(0.2f, 0.5f, 0.2f, 1.0);
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
    //    glm::mat4 modelTransform =
    //    models[i].transformation.getTransformationMat();
    //    shaderProgram.uniformSetMat4fv("model", modelTransform);
    //    glm::mat4 viewTransform = scene.camera.getViewMatrix();
    //    shaderProgram.uniformSetMat4fv("view", viewTransform);
    //    glm::mat4 projectionTransform = scene.camera.getProjectionMatrix();
    //    shaderProgram.uniformSetMat4fv("projectionTransform",
    //    projectionTransform);
    //    glm::mat4 modelTransform = glm::mat4(1.0f);
    //    shaderProgram.uniformSetMat4fv("model", modelTransform);
    //    glm::mat4 viewTransform = glm::mat4(1.0f);
    //    shaderProgram.uniformSetMat4fv("view", viewTransform);
    //    glm::mat4 projectionTransform = glm::mat4(1.0f);
    //    shaderProgram.uniformSetMat4fv("projection", projectionTransform);

    //    for (int i = 0; i < textures.size(); ++i) {
    //      shaderProgram.uniformSet1Int("texture" + std::to_string(i), i);
    //      glActiveTexture(GL_TEXTURE0);
    //      glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
    //    }
    glDrawArrays(GL_TRIANGLES, index, models[i].numOfVertices);
    index += models[i].numOfVertices;
  }

  for (int i = 0; i < vaoMaxIndex; ++i) {
    glDisableVertexAttribArray(i);
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
