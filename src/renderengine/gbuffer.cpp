
#include "gbuffer.h"
#include "shader.h"
#include <iostream>

GBuffer::GBuffer() {
  FBO = 0;
  depthRBO = 0;
}

void GBuffer::cleanUp() {
  if (FBO != 0) {
    glDeleteFramebuffers(1, &FBO);
  }
  if (gTextures.size() > 0) {
    for (GBufferTexture texture : gTextures) {
      glDeleteTextures(1, &texture.textureID);
    }
  }
  if (depthRBO != 0) {
    glDeleteTextures(1, &depthRBO);
  }
}

bool GBuffer::init(int scrWidth, int scrHeight,
                   std::vector<GBufferTexture> &textures, bool needDepthRBO) {
  // creat fbo
  glad_glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  int size = textures.size();
  unsigned int attachments[size];
  for (unsigned int i = 0; i < size; ++i) {
    GBufferTexture &texture = textures[i];
    glGenTextures(1, &texture.textureID);
    glBindTexture(GL_TEXTURE_2D, texture.textureID);
    switch (texture.type) {
    case GBUFFER_TEXTURE_TYPE_POSITION:
    case GBUFFER_TEXTRURE_NORMAL:
    case GBUFFER_TEXTURE_DIFFUSE:
    case GBUFFER_TEXTURE_TEXCOORDS: {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, scrWidth, scrHeight, 0, GL_RGB,
                   GL_FLOAT, NULL);
      break;
    }
    case GBUFFER_TEXTURE_SPECULAR_SHININESS: {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrWidth, scrHeight, 0,
                   GL_RGBA, GL_FLOAT, NULL);
      break;
    }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                           GL_TEXTURE_2D, texture.textureID, 0);
    attachments[i] = GL_COLOR_ATTACHMENT0 + i;
  }
  glDrawBuffers(size, attachments);
  gTextures = textures;

  // Depth RBO
  if (needDepthRBO) {
    glGenRenderbuffers(1, &depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scrWidth,
                          scrHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthRBO);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "gBuffer fbo not complete!" << std::endl;
    return false;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return true;
}

void GBuffer::bind() { glBindFramebuffer(GL_FRAMEBUFFER, FBO); }

void GBuffer::bindForRead() { glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO); }

void GBuffer::bindForWrite() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); }

void GBuffer::configure(ShaderProgram &shaderProgram) {
  for (unsigned int i = 0; i < gTextures.size(); ++i) {
    GBufferTexture gBufferTexture = gTextures[i];
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, gBufferTexture.textureID);
    shaderProgram.uniformSetInt(gBufferTexture.uniformName, i);
  }
}