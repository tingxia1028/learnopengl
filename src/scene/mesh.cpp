
#include "mesh.h"
#include <iostream>
Mesh::Mesh(std::string name, const std::vector<Vertex> &vertices,
           const std::vector<unsigned int> &indices,
           const std::vector<Material> &materials)
    : name(name), vertices(vertices), indices(indices), materials(materials) {
  loadData();
}

void Mesh::loadData() {
  create();
  storeData();
  unbind();
}

void Mesh::draw(ShaderProgram &shaderProgram, bool withMaterials,
                std::vector<Light *> &lights) {
  glBindVertexArray(VAO);
  for (unsigned int i = 0; i < ATTRIBUTE_NUM; ++i) {
    glEnableVertexAttribArray(i);
  }

  for (Light *light : lights) {
    light->activeShadowTex();
  }

  textureIndex = lights.size();

  if (withMaterials) {
    for (unsigned int j = 0; j < materials.size(); ++j) {
      materials[j].configure(shaderProgram, j, textureIndex);
    }
  }

  if (!indices.empty()) {
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
  }

  for (unsigned int i = 0; i < ATTRIBUTE_NUM; ++i) {
    glDisableVertexAttribArray(i);
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glActiveTexture(GL_TEXTURE0);
  textureIndex = 0;
}

void Mesh::create() {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
}

void Mesh::storeData() {
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);

  // EBO
  if (!indices.empty()) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);
  }

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, textureCoord));
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, tangent));
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, bitangent));
}

void Mesh::unbind() {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}