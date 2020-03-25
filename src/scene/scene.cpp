
#include "scene.h"
#include "model.h"
#include <iostream>
#include <map>
Scene::Scene(std::vector<Model> &models, Camera *camera,
             std::vector<Light *> &lights, SkyBox *skyBox)
    : models(models), camera(camera), lights(lights), skyBox(skyBox) {}

void Scene::cleanUp() {
  for (unsigned int i = 0; i < models.size(); ++i) {
    std::map<std::string, TextureData> maps = models[i].loadedTextures;
    for (std::map<std::string, TextureData>::iterator it = maps.begin();
         it != maps.end(); ++it) {
      glDeleteTextures(1, &it->second.textureID);
    }
    for (Mesh &mesh : models[i].meshes) {
      glDeleteVertexArrays(1, &mesh.VAO);
      glDeleteBuffers(1, &mesh.VBO);
      glDeleteBuffers(1, &mesh.EBO);
    }
  }
}

void Scene::generateFBO(int scrWidth, int scrHeight) {
  // floating point color buffer
  glGenTextures(1, &deferredTex);
  glBindTexture(GL_TEXTURE_2D, deferredTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA,
               GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // depth buffer
  glGenRenderbuffers(1, &deferredRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, deferredRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scrWidth,
                        scrHeight);

  // FBO
  glGenFramebuffers(1, &deferredFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         deferredTex, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, deferredRBO);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "scene deferred render FBO not complete!" << std::endl;
  }

  // unbind
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
