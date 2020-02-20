
#include "loader.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

void Loader::loadToVAO(float *data, int size, GLuint *indices,
                       int sizeOfIndices, const int vertexNum,
                       const int dimension) {
  unsigned int VAO;
  createVAO(&VAO, 1);
  storeDataInAttributeList(data, size, indices, sizeOfIndices, dimension);
  unbind();

  rawModel = RawModel(VAO, vertexNum);
}

void Loader::cleanup() {
  glDeleteVertexArrays(vaos.size(), &vaos.front());
  vaos.clear();
  glDeleteBuffers(vbos.size(), &vbos.front());
  vbos.clear();
  glDeleteBuffers(ebos.size(), &ebos.front());
  ebos.clear();
}

void Loader::createVAO(GLuint *vao, int size) {
  glGenVertexArrays(size, vao);
  vaos.push_back(*vao);
  glBindVertexArray(*vao);
}

void Loader::storeDataInAttributeList(float *data, int size, GLuint *indices,
                                      int sizeOfIndices, const int dimension) {
  GLuint VBO;
  glGenBuffers(1, &VBO);
  vbos.push_back(VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  // VEO
  GLuint VEO;
  glGenBuffers(1, &VEO);
  ebos.push_back(VEO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

  // VAO
  glVertexAttribPointer(0, dimension, GL_FLOAT, GL_FALSE,
                        dimension * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
}
void Loader::unbind() {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
