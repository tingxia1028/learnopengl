
#ifndef OPENGL_SKYBOX_H
#define OPENGL_SKYBOX_H

#include <glad/glad.h>
#include <string>
#include <vector>
class SkyBox {
public:
  SkyBox() = default;
  ~SkyBox() = default;
  SkyBox(std::vector<std::string> &textures);
  GLuint VAO;
  GLuint textureID;

private:
  void loadTextures(std::vector<std::string> &textures);
  void loadBoxModel();
};

#endif // OPENGL_SKYBOX_H
