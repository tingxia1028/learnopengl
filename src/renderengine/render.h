
#ifndef OPENGL_RENDER_H
#define OPENGL_RENDER_H

#include "../scene/scene.h"
#include "displaymanager.h"
#include "shader.h"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

class Render {
public:
  static void prepare(Camera *camera, DisplayManager &displayManager);
  static void renderShadowMap(Scene &scene, ShaderProgram &shaderProgram);
  static void debugRenderShadowMap(Scene &scene, ShaderProgram &shaderProgram);
  static void render(Scene &scene, ShaderProgram &shaderProgram,
                     bool withLights = false, bool withMaterials = false);
  static void renderSkyBox(Scene &scene, ShaderProgram &shaderProgram);

private:
  static void configureLights(std::vector<Light *> &lights,
                              ShaderProgram &shaderProgram);
};

#endif // OPENGL_RENDER_H
