
#include "loader.h"
#include "../model/Scene.h"
#include "../model/model.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "stb_image.h"

#include <iostream>

VAOLoader::VAOLoader(Scene *scene) : AbstractLoader(scene){};

void VAOLoader::cleanup() {
  if (vaos.size() > 0) {
    glDeleteVertexArrays(vaos.size(), &vaos.front());
  }
  vaos.clear();
  if (vbos.size() > 0) {
    glDeleteBuffers(vbos.size(), &vbos.front());
  }
  vbos.clear();
  if (ebos.size() > 0) {
    glDeleteBuffers(ebos.size(), &ebos.front());
  }
  ebos.clear();
}

void VAOLoader::create() {
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  vaos.push_back(VAO);
  (*scene).vaoID = VAO;
}

void VAOLoader::storeData() {
  std::vector<float> vertices;
  std::vector<GLuint> indices;
  std::vector<float> textureCoords;
  std::vector<float> normals;
  for (int i = 0; i < (*scene).models.size(); ++i) {
    Model model = (*scene).models[i];
    vertices.insert(vertices.end(), model.vertices.begin(),
                    model.vertices.end());
    if (model.indices.size() > 0) {
      indices.insert(indices.end(), model.indices.begin(), model.indices.end());
    }
    textureCoords.insert(textureCoords.end(), model.textureCoords.begin(),
                         model.textureCoords.end());
    normals.insert(normals.end(), model.normals.begin(), model.normals.end());
  }

  GLuint VBO;
  glGenBuffers(1, &VBO);
  vbos.push_back(VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               (vertices.size() + textureCoords.size() + normals.size()) *
                   sizeof(float),
               nullptr, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float),
                  &vertices[0]);
  glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                  textureCoords.size() * sizeof(float), &textureCoords[0]);
  glBufferSubData(GL_ARRAY_BUFFER,
                  (vertices.size() + textureCoords.size()) * sizeof(float),
                  normals.size() * sizeof(float), &normals[0]);

  //   VEO
  if (!indices.empty()) {
    GLuint VEO;
    glGenBuffers(1, &VEO);
    ebos.push_back(VEO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices,
                 GL_STATIC_DRAW);
  }

  // VAO_0
  int vaoMaxIndex = 0;
  glVertexAttribPointer(vaoMaxIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void *)0);
  glVertexAttribPointer(++vaoMaxIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)(vertices.size() * sizeof(float)));
  glVertexAttribPointer(
      ++vaoMaxIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
      (void *)((vertices.size() + textureCoords.size()) * sizeof(float)));
  (*scene).maxVaoIndex = vaoMaxIndex + 1;
}

void VAOLoader::unbind() {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TextureLoader::TextureLoader(Scene *scene) : AbstractLoader(scene){};

void TextureLoader::load() {
  for (int i = 0; i < (*scene).models.size(); ++i) {
    Model *currentModel = &(*scene).models[i];
    for (int i = 0; i < (*currentModel).materials.size(); ++i) {
      Material &material = (*currentModel).materials[i];
      for (int j = 0; j < material.textures.size(); ++j) {
        textureData = &material.textures[i];
        create();
        storeData();
        unbind();
      }
    }
  }
}

void TextureLoader::create() {
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  (*textureData).textureID = textureID;
  textures.push_back(textureID);
}

void TextureLoader::storeData() {
  // tell stb_image.h to flip loaded texture's on the y-axis.
  // cause opengl texture coordinates start from top left
  stbi_set_flip_vertically_on_load(true);
  int width, height, nChannels;
  unsigned char *data = stbi_load((*textureData).texturePath.c_str(), &width,
                                  &height, &nChannels, 0);
  if (data) {
    // dst format  & src format
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "load texture image failed : " << (*textureData).texturePath
              << std::endl;
  }
  stbi_image_free(data);
}

void TextureLoader::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void TextureLoader::cleanup() {
  if (textures.size() > 0) {
    glDeleteTextures(textures.size(), &textures.front());
  }
  textures.clear();
}
