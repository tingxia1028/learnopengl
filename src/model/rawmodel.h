
#ifndef OPENGL_RAWMODEL_H
#define OPENGL_RAWMODEL_H
#include <glad/glad.h>
class RawModel {
public:
  GLuint vaoID;
  GLuint textureID;
  int vertexCounts;
  int vaoMaxIndex = 1;
};

#endif // OPENGL_RAWMODEL_H
