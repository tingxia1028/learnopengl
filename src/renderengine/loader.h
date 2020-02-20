
#ifndef OPENGL_LOADER_H
#define OPENGL_LOADER_H

#include "rawmodel.h"
#include <glad/glad.h>
#include <vector>

class Loader {
public:
  void loadToVAO(float *data, int size, GLuint *indices, int sizeOfIndices,
                 const int vertexNum, const int dimension);
  void cleanup();
  RawModel rawModel = RawModel();

private:
  std::vector<GLuint> vaos;
  std::vector<GLuint> vbos;
  std::vector<GLuint> ebos;
  void createVAO(GLuint *vaos, int size);
  void storeDataInAttributeList(float *data, int size, GLuint *indices,
                                int sizeOfIndices, const int dimension);
  void unbind();
};

#endif // OPENGL_LOADER_H
