
#ifndef OPENGL_GBUFFER_H
#define OPENGL_GBUFFER_H

#include "shader.h"
#include <glad/glad.h>
#include <string>
#include <vector>

enum GBufferTextureType {
  GBUFFER_TEXTURE_TYPE_POSITION,
  GBUFFER_TEXTRURE_NORMAL,
  GBUFFER_TEXTURE_TEXCOORDS,
  GBUFFER_TEXTURE_DIFFUSE,
  GBUFFER_TEXTURE_SPECULAR_SHININESS
};

struct GBufferTexture {
public:
  GBufferTextureType type;
  GLuint textureID;
  std::string uniformName;
  GBufferTexture(GBufferTextureType type, const std::string &uniformName)
      : type(type), uniformName(uniformName) {}
};

class GBuffer {
public:
  GBuffer();
  virtual ~GBuffer() = default;
  void cleanUp();
  bool init(int scrWidth, int scrHeight, std::vector<GBufferTexture> &textures,
            bool needDepthRBO);
  void bind();
  void bindForRead();
  void bindForWrite();
  void configure(ShaderProgram &shaderProgram);

private:
  GLuint FBO;
  std::vector<GBufferTexture> gTextures;
  GLuint depthRBO;
};

#endif // OPENGL_GBUFFER_H
