
#include "directionallight.h"
#include "../renderengine/render.h"
DirectionalLight::DirectionalLight(const glm::vec3 &ambient,
                                   const glm::vec3 &diffuse,
                                   const glm::vec3 &specular,
                                   const LightType lightType,
                                   const glm::vec3 &direction)
    : Light(-direction, ambient, diffuse, specular, lightType),
      direction(direction) {
  Camera lightView(position, direction, 20.0f, 20.0f, 1.0f, 7.5f);
  lightSpaceTrans =
      lightView.getProjectionMatrix(false) * lightView.getViewMatrix();
  genShadowMap();
}

void DirectionalLight::configure(ShaderProgram &shaderProgram,
                                 std::string lightType, std::string index,
                                 int depthMapIndex) {
  Light::configure(shaderProgram, lightType, index, depthMapIndex);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].direction",
                                direction);
}

void DirectionalLight::genShadowMap() {
  // shadow map texture
  glGenTextures(1, &depthMapTex);
  glBindTexture(GL_TEXTURE_2D, depthMapTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
               SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // framebuffer
  glGenFramebuffers(1, &shadowMapFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depthMapTex, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  // unbind
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
