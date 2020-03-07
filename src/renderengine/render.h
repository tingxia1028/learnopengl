
#ifndef OPENGL_RENDER_H
#define OPENGL_RENDER_H

#include "../scene/scene.h"
#include "shader.h"
class Render {
public:
  void prepare();
  void render(Scene &scene, ShaderProgram &shaderProgram);
};

#endif // OPENGL_RENDER_H
