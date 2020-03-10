
#include "skybox.h"
#include "../renderengine/stb_image.h"
#include <iostream>

SkyBox::SkyBox(std::vector<std::string> &textures) {
  loadBoxModel();
  loadTextures(textures);
}

void SkyBox::loadBoxModel() {

  float skyBoxVertices[] = {
      -10.0f, 10.0f,  -10.0f, -10.0f, -10.0f, -10.0f, 10.0f,  -10.0f, -10.0f,
      10.0f,  -10.0f, -10.0f, 10.0f,  10.0f,  -10.0f, -10.0f, 10.0f,  -10.0f,
      -10.0f, -10.0f, 10.0f,  -10.0f, -10.0f, -10.0f, -10.0f, 10.0f,  -10.0f,
      -10.0f, 10.0f,  -10.0f, -10.0f, 10.0f,  10.0f,  -10.0f, -10.0f, 10.0f,
      10.0f,  -10.0f, -10.0f, 10.0f,  -10.0f, 10.0f,  10.0f,  10.0f,  10.0f,
      10.0f,  10.0f,  10.0f,  10.0f,  10.0f,  -10.0f, 10.0f,  -10.0f, -10.0f,
      -10.0f, -10.0f, 10.0f,  -10.0f, 10.0f,  10.0f,  10.0f,  10.0f,  10.0f,
      10.0f,  10.0f,  10.0f,  10.0f,  -10.0f, 10.0f,  -10.0f, -10.0f, 10.0f,
      -10.0f, 10.0f,  -10.0f, 10.0f,  10.0f,  -10.0f, 10.0f,  10.0f,  10.0f,
      10.0f,  10.0f,  10.0f,  -10.0f, 10.0f,  10.0f,  -10.0f, 10.0f,  -10.0f,
      -10.0f, -10.0f, -10.0f, -10.0f, -10.0f, 10.0f,  10.0f,  -10.0f, -10.0f,
      10.0f,  -10.0f, -10.0f, -10.0f, -10.0f, 10.0f,  10.0f,  -10.0f, 10.0f};

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertices), &skyBoxVertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkyBox::loadTextures(std::vector<std::string> &textures) {
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < textures.size(); ++i) {
    unsigned char *data =
        stbi_load(textures[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap texture failed to load at path: " << textures[i]
                << std::endl;
      stbi_image_free(data);
    }
  }
}