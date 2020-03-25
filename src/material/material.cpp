
#include "material.h"

Material::Material(const glm::vec3 &diffuse, const glm::vec3 &specular,
                   float shininess, const std::vector<TextureData> &textures)
    : diffuse(diffuse), specular(specular), shininess(shininess),
      textures(textures) {}

void Material::configure(ShaderProgram &shaderProgram, int materialIndex,
                         int textureIndex) {
  std::string index = std::to_string(materialIndex);
  shaderProgram.uniformSetFloat("materials[" + index + "].shininess",
                                shininess);
  shaderProgram.uniformSetVec3F("materials[" + index + "].diffuseColor",
                                diffuse);
  shaderProgram.uniformSetVec3F("materials[" + index + "].specularColor",
                                specular);
  shaderProgram.uniformSetBool("materials[" + index + "].hasDiffuseTex",
                               hasDiffuseTex);
  shaderProgram.uniformSetBool("materials[" + index + "].hasSpecularTex",
                               hasSpecularTex);
  shaderProgram.uniformSetBool("materials[" + index + "].hasNormalMap",
                               hasNormalMap);
  shaderProgram.uniformSetBool("materials[" + index + "].hasDepthMap",
                               hasDepthMap);
  for (unsigned int k = 0; k < textures.size(); ++k, ++textureIndex) {
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    TextureData textureData = textures[k];
    shaderProgram.uniformSetInt("materials[" + index + "]." +
                                    TexTypeToString(textureData.type),
                                textureIndex);
    glBindTexture(GL_TEXTURE_2D, textureData.textureID);
  }
}
