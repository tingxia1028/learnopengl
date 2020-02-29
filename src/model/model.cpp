
#include "model.h"
#include <glm/ext/matrix_transform.hpp>
Model::Model(std::vector<float> &vertices, int numOfVertices,
             std::vector<GLuint> &indices, std::vector<TextureData> &textures,
             std::vector<float> &textureCoords, Transformation transformation)
    : vertices(vertices), numOfVertices(numOfVertices), indices(indices),
      textures(textures), textureCoords(textureCoords),
      transformation(transformation) {}
