
#ifndef OPENGL_SCENE_H
#define OPENGL_SCENE_H

#include "../camera/camera.h"
#include "../light/light.h"
#include "../renderengine/gbuffer.h"
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
  void generateBlurFBO(int scrWidth, int scrHeight);

  std::vector<Model> models;
  Camera *camera;
  std::vector<Light *> lights;
  SkyBox *skyBox;
  GBuffer gBuffer;
  GLuint deferredFBO;
  std::vector<GLuint> deferredTex;
  GLuint deferredRBO;
  GLuint pingpongFBO[2];
  GLuint pingpongColorBuffers[2];
};

#endif // OPENGL_SCENE_H
