
#include "scene.h"
#include "model.h"
#include <iostream>
#include <map>
Scene::Scene(std::vector<Model> &models, Camera *camera,
             std::vector<Light *> &lights, SkyBox *skyBox)
    : models(models), camera(camera), lights(lights), skyBox(skyBox) {}

void Scene::cleanUp() {
  for (unsigned int i = 0; i < models.size(); ++i) {
    std::map<std::string, Texture> maps = models[i].loadedTextures;
    for (std::map<std::string, Texture>::iterator it = maps.begin();
         it != maps.end(); ++it) {
      it->second.cleanUp();
    }
    for (Mesh &mesh : models[i].meshes) {
      mesh.cleanUp();
    }
    gBuffer.cleanUp();
  }
}

void Scene::generateFBO(int scrWidth, int scrHeight) {
  // FBO
  glGenFramebuffers(1, &deferredFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);

  // floating point color buffer
  deferredTex = std::vector<GLuint>(2, 0);
  glGenTextures(2, &deferredTex[0]);
  for (unsigned int i = 0; i < 2; ++i) {
    glBindTexture(GL_TEXTURE_2D, deferredTex[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0, GL_RGBA,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                           GL_TEXTURE_2D, deferredTex[i], 0);
  }
  unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(2, attachments);

  // create depth buffer (renderbuffer)
  glGenRenderbuffers(1, &deferredRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, deferredRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scrWidth,
                        scrHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, deferredRBO);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "scene deferred render fbo not complete!" << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Scene::generateBlurFBO(int scrWidth, int scrHeight) {
  glGenFramebuffers(2, pingpongFBO);
  glGenTextures(2, pingpongColorBuffers);
  for (int i = 0; i < 2; ++i) {
    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
    glBindTexture(GL_TEXTURE_2D, pingpongColorBuffers[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scrWidth, scrHeight, 0, GL_RGB,
                 GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           pingpongColorBuffers[i], 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cout << "scene blur render fbo not complete!" << std::endl;
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
