
#ifndef OPENGL_GBUFFER_H
#define OPENGL_GBUFFER_H

#include <glad/glad.h>
#include <string>
#include <vector>

enum GBufferTextureType {
  GBUFFER_TEXTURE_TYPE_POSITION,
  GBUFFER_TEXTRURE_NORMAL,
  GBUFFER_TEXTURE_TEXCOORDS,
  GBUFFER_TEXTURE_DIFFUSE,
  GBUFFER_TEXTURE_SPECULAR
};

struct GBufferTexture {
  GBufferTextureType type;
  GLuint textureID;
  std::string uniformName;
};

class GBuffer {
public:
  GBuffer();
  virtual ~GBuffer();
  bool init(int scrWidth, int scrHeight, std::vector<GBufferTexture> &textures,
            bool needDepthRBO);
  bool bind();

private:
  GLuint FBO;
  std::vector<GBufferTexture> textures;
  GLuint depthRBO;
};

#endif // OPENGL_GBUFFER_H
