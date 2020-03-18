
#include "pointlight.h"
#include "../renderengine/render.h"
#include <glm/gtc/matrix_transform.hpp>
PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                       const glm::vec3 &specular, const LightType lightType,
                       const glm::vec3 &position, float constTerm,
                       float linearTerm, float quadraticTerm)
    : Light(position, ambient, diffuse, specular, lightType),
      constTerm(constTerm), linearTerm(linearTerm),
      quadraticTerm(quadraticTerm) {
  genShadowMap();
}

void PointLight::configure(ShaderProgram &shaderProgram, std::string lightType,
                           std::string index) {
  Light::configure(shaderProgram, lightType, index);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].constant",
                                constTerm);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].linear",
                                linearTerm);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].quadratic",
                                quadraticTerm);
  shaderProgram.uniformSetVec3F(lightType + "s[" + index + "].position",
                                position);
  shaderProgram.uniformSetFloat(lightType + "s[" + index + "].farPlane",
                                farPlane);
}

void PointLight::genShadowMap() {
  // cube textures
  glGenTextures(1, &depthMapTex);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapTex);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  // framebuffer
  glGenFramebuffers(1, &shadowMapFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapTex, 0);
  glDrawBuffer(GL_NONE);
  glDrawBuffer(GL_NONE);

  // unbind
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void PointLight::configureShadowMatrices(ShaderProgram &shaderProgram) {
  float nearPlane = 1.0f;
  farPlane = 25.0f;
  glm::mat4 projection = glm::perspective(
      glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT,
      nearPlane, farPlane);
  std::vector<glm::mat4> shadowTransforms;
  shadowTransforms.push_back(
      projection * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f),
                               glm::vec3(0.0, -1.0f, 0.0f)));
  shadowTransforms.push_back(
      projection * glm::lookAt(position,
                               position + glm::vec3(-1.0f, 0.0f, 0.0f),
                               glm::vec3(0.0, -1.0f, 0.0f)));
  shadowTransforms.push_back(
      projection * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f),
                               glm::vec3(0.0, 0.0f, 1.0f)));
  shadowTransforms.push_back(
      projection * glm::lookAt(position,
                               position + glm::vec3(0.0f, -1.0f, 0.0f),
                               glm::vec3(0.0, 0.0f, -1.0f)));
  shadowTransforms.push_back(
      projection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec3(0.0, -1.0f, 0.0f)));
  shadowTransforms.push_back(
      projection * glm::lookAt(position,
                               position + glm::vec3(0.0f, 0.0f, -1.0f),
                               glm::vec3(0.0, -1.0f, 0.0f)));
  for (unsigned int i = 0; i < 6; ++i) {
    shaderProgram.uniformSetMat4("shadowMatrices[" + std::to_string(i) + "]",
                                 shadowTransforms[i]);
  }
  shaderProgram.uniformSetFloat("far", farPlane);
  shaderProgram.uniformSetVec3F("lightPos", position);
}

void PointLight::activeShadowTex() {
  glActiveTexture(depthMapIndex);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapIndex);
}
