
#ifndef OPENGL_SCENE_H
#define OPENGL_SCENE_H

#include "../camera/camera.h"
#include "model.h"
#include <glad/glad.h>
class Scene {
public:
  GLuint vaoID;
  int maxVaoIndex;
  std::vector<Model> models;
  Camera camera;
  Scene(std::vector<Model> &models, Camera camera);
};

#endif // OPENGL_SCENE_H
