
#include "texture.h"
#include "../renderengine/stb_image.h"
#include <iostream>

Texture::Texture() { textureObj = 0; }

Texture::~Texture() {}

void Texture::cleanUp() { glDeleteTextures(1, &textureObj); }

Texture::Texture(GLenum textureTarget, const std::string &fileName,
                 TextureType &textureType)
    : textureTarget(textureTarget), fileName(fileName), type(textureType) {}

bool Texture::load() {
  int width, height, nrComponents;
  unsigned char *data =
      stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
  bool isDiffuse = (type == TextureType::DIFFUSE);
  if (data) {
    GLenum format;
    GLenum internalFormat;
    if (nrComponents == 1) {
      format = GL_RED;
      internalFormat = GL_RED;
    } else if (nrComponents == 3) {
      format = GL_RGB;
      internalFormat = isDiffuse ? GL_SRGB : GL_RGB16F;
    } else if (nrComponents == 4) {
      format = GL_RGBA;
      internalFormat = isDiffuse ? GL_SRGB_ALPHA : GL_RGBA16F;
    }

    glGenTextures(1, &textureObj);
    glBindTexture(textureTarget, textureObj);
    glTexImage2D(textureTarget, 0, internalFormat, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(textureTarget);

    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(textureTarget, 0);

    stbi_image_free(data);
    return true;
  } else {
    std::cout << "Texture failed to load at path: " << fileName << std::endl;
    stbi_image_free(data);
    return false;
  }
}

void Texture::bind(GLenum textureUnit) {
  glActiveTexture(textureUnit);
  glBindTexture(textureTarget, textureObj);
}

std::string Texture::TexTypeToString() {
  std::string table[] = {"diffuse", "specular", "normal", "depth"};
  return table[(int)type];
}
