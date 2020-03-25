
#ifndef OPENGL_SCENE_H
#define OPENGL_SCENE_H

#include "../camera/camera.h"
#include "../light/light.h"
#include "model.h"
#include "skybox.h"
#include <glad/glad.h>
class Scene {
public:
  Scene() = default;
  ~Scene() = default;
  Scene(std::vector<Model> &models, Camera *camera,
        std::vector<Light *> &lights, SkyBox *skyBox);
  void cleanUp();
  void generateFBO(int scrWidth, int scrHeight);

  std::vector<Model> models;
  Camera *camera;
  std::vector<Light *> lights;
  SkyBox *skyBox;
  GLuint deferredFBO;
  GLuint deferredTex;
  GLuint deferredRBO;
};

#endif // OPENGL_SCENE_H
