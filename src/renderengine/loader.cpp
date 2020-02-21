
#include "loader.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "stb_image.h"
#include <iostream>

VAOLoader::VAOLoader(Data<float> vertexData, Data<float> textureCoords,
                     Data<GLuint> indicesData, const int vertexNum,
                     RawModel *rawModel)
    : vertexData(vertexData), textureCoords(textureCoords),
      indicesData(indicesData), vertexNum(vertexNum),
      AbstractLoader(rawModel){};

void VAOLoader::cleanup() {
  glDeleteVertexArrays(vaos.size(), &vaos.front());
  vaos.clear();
  glDeleteBuffers(vbos.size(), &vbos.front());
  vbos.clear();
  glDeleteBuffers(ebos.size(), &ebos.front());
  ebos.clear();
}

void VAOLoader::create() {
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  vaos.push_back(VAO);
  (*rawModel).vaoID = VAO;
}

void VAOLoader::storeData() {
  GLuint VBO;
  glGenBuffers(1, &VBO);
  vbos.push_back(VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               vertexData.sizeOfData + textureCoords.sizeOfData, nullptr,
               GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.sizeOfData, vertexData.data);
  glBufferSubData(GL_ARRAY_BUFFER, vertexData.sizeOfData,
                  textureCoords.sizeOfData, textureCoords.data);

  // VEO
  GLuint VEO;
  glGenBuffers(1, &VEO);
  ebos.push_back(VEO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.sizeOfData,
               indicesData.data, GL_STATIC_DRAW);

  // VAO_0
  int vaoMaxIndex = 0;
  glVertexAttribPointer(vaoMaxIndex, vertexData.dataDimension, GL_FLOAT,
                        GL_FALSE, vertexData.dataDimension * sizeof(float),
                        (void *)0);
  glVertexAttribPointer(++vaoMaxIndex, textureCoords.dataDimension, GL_FLOAT,
                        GL_FALSE, textureCoords.dataDimension * sizeof(float),
                        (void *)(vertexData.sizeOfData));
  (*rawModel).vaoMaxIndex = vaoMaxIndex + 1;
}

void VAOLoader::unbind() {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TextureLoader::TextureLoader(std::string tPath, RawModel *rawModel)
    : texturePath(tPath), AbstractLoader(rawModel){};

void TextureLoader::create() {
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  (*rawModel).textureID = textureID;
}

void TextureLoader::storeData() {
  // tell stb_image.h to flip loaded texture's on the y-axis.
  // cause opengl texture coordinates start from top left
  stbi_set_flip_vertically_on_load(true);
  int width, height, nChannels;
  unsigned char *data =
      stbi_load(texturePath.c_str(), &width, &height, &nChannels, 0);
  if (data) {
    // dst format  & src format
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "load texture image failed" << std::endl;
  }
  stbi_image_free(data);
}

void TextureLoader::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void TextureLoader::cleanup() {
  glDeleteTextures(textures.size(), &textures.front());
  textures.clear();
}
