
#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H

#include "../material/material.h"
#include "../transformation/rotate.h"
#include "../transformation/transformation.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Model {
public:
  Model(std::vector<float> &vertices, int numOfVertices,
        std::vector<GLuint> &indices, std::vector<float> &textureCoords,
        Transformation transformation, std::vector<float> &normals,
        std::vector<Material> &materials);
  Model(){};
  // vertex
  std::vector<float> vertices;
  int numOfVertices;
  std::vector<GLuint> indices;
  std::vector<float> normals;

  // materials
  std::vector<Material> materials;
  std::vector<float> textureCoords;

  // transformation
  Transformation transformation;
};

#endif // OPENGL_MODEL_H
