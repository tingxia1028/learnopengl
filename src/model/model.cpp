
#include "model.h"

Model::Model(std::vector<float> &vertices, int numOfVertices,
             std::vector<GLuint> &indices, std::vector<float> &textureCoords,
             Transformation transformation, std::vector<float> &normals,
             std::vector<Material> &materials)
    : vertices(vertices), numOfVertices(numOfVertices), indices(indices),
      textureCoords(textureCoords), transformation(transformation),
      normals(normals), materials(materials) {}
