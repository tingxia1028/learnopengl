
#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H

#include "../transformation/rotate.h"
#include "../transformation/transformation.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct TextureData {
  std::string texturePath;
  GLuint textureID = 0;
  TextureData(std::string texturePath) : texturePath(texturePath){};
  TextureData(){};
};

class Model {
public:
  Model(std::vector<float> &vertices, int numOfVertices,
        std::vector<GLuint> &indices, std::vector<TextureData> &textureData,
        std::vector<float> &textureCoords, Transformation transformation,
        std::vector<float> &normals);
  Model(){};
  // vertex
  std::vector<float> vertices;
  int numOfVertices;
  std::vector<GLuint> indices;
  std::vector<float> normals;

  // texture
  std::vector<TextureData> textures;
  std::vector<float> textureCoords;

  // transformation
  Transformation transformation;
};

#endif // OPENGL_MODEL_H
