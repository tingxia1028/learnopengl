
#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture {
public:
  Texture();
  ~Texture();
  enum TextureType { DIFFUSE, SPECULAR, NORMAL, HEIGHT };
  Texture(GLenum textureTarget, const std::string &fileName,
          TextureType &textureType);
  bool load();
  void bind(GLenum textureUnit);
  std::string TexTypeToString();
  void cleanUp();

private:
  GLenum textureTarget;
  GLuint textureObj;
  std::string fileName;
  TextureType type;
};

#endif // OPENGL_TEXTURE_H
