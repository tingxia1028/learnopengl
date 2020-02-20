
#ifndef OPENGL_RAWMODEL_H
#define OPENGL_RAWMODEL_H
#include <glad/glad.h>
class RawModel {
public:
  GLuint vaoID;
  int vertexCounts;
  int vaoMaxIndex = 1;
  RawModel(){};
  RawModel(GLuint vId, int vCount);
};

#endif // OPENGL_RAWMODEL_H
