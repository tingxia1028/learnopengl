
#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H

#include "../light/light.h"
#include "../material/material.h"
#include "../transformation/rotate.h"
#include "../transformation/transformation.h"
#include "mesh.h"
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

class Model {
public:
  Model() = default;
  ~Model() = default;
  Model(const std::string &path, Transformation &transformation);
  void draw(ShaderProgram &shaderProgram, std::vector<Light *> &lights,
            bool withMaterials = false);

  std::vector<Mesh> meshes;
  Transformation transformation;
  std::map<std::string, Texture> loadedTextures;
  std::string directory;

private:
  void loadModel(const std::string &path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            Texture::TextureType typeName);
  glm::vec3 transformAIcolor(aiColor3D aiColor3D);
};

#endif // OPENGL_MODEL_H
