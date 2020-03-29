
#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H

#include "../light/light.h"
#include "../material/material.h"
#include "../renderengine/shader.h"
#include <glm/vec3.hpp>
#include <vector>
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoord;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

constexpr unsigned int ATTRIBUTE_NUM = 3;

class Mesh {
public:
  Mesh() = default;
  ~Mesh() = default;
  void cleanUp();
  Mesh(std::string name, const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices,
       const std::vector<Material> &materials);
  void draw(ShaderProgram &shaderProgram, bool withMaterials,
            std::vector<Light *> &lights);

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Material> materials;
  unsigned int VAO, VBO, EBO;
  std::string name;

private:
  void loadData();
  void create();
  void storeData();
  void unbind();
  void configureMaterials(ShaderProgram &shaderProgram);
  int textureIndex = 0;
};

#endif // OPENGL_MESH_H
