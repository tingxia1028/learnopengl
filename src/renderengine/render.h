
#ifndef OPENGL_RENDER_H
#define OPENGL_RENDER_H

#include "../scene/scene.h"
#include "displaymanager.h"
#include "shader.h"
#include <set>

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

class Render {
public:
  static void prepare(Camera *camera, DisplayManager &displayManager);
  static void renderShadowMap(Scene &scene, ShaderProgram &shaderProgram,
                              std::set<LightType> &lightTypes);
  static void debugRenderShadowMap(Scene &scene, ShaderProgram &shaderProgram);
  static void render(Scene &scene, ShaderProgram &shaderProgram,
                     bool withLights = false, bool withMaterials = false,
                     bool withShadowMap = false);
  static void renderSkyBox(Scene &scene, ShaderProgram &shaderProgram);
  static void renderLight(ShaderProgram &shader, glm::vec3 &lightPos,
                          glm::vec3 &diffuse);
  static void deferredRender(ShaderProgram &shader, Scene &scene);
  static void renderBlur(ShaderProgram &shader, Scene &scene);
  static void renderGBuffer(ShaderProgram &shaderProgram, Scene &scene);
  static void renderLightPass(ShaderProgram &shaderProgram, Scene &scene);
  static GLuint cubeVAO;
  static GLuint cubeVBO;
  static GLuint quadVAO;
  static GLuint quadVBO;

private:
  static void configureLights(std::vector<Light *> &lights,
                              ShaderProgram &shaderProgram);
  static void renderCube();
  static void renderQuad();
};

#endif // OPENGL_RENDER_H
