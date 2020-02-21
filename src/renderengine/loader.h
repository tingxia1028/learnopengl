
#ifndef OPENGL_LOADER_H
#define OPENGL_LOADER_H

#include "../model/rawmodel.h"
#include <glad/glad.h>
#include <string>
#include <vector>

class AbstractLoader {
public:
  AbstractLoader(RawModel *rawModel) : rawModel(rawModel){};
  virtual void load() {
    create();
    storeData();
    unbind();
  };
  virtual void cleanup() = 0;
  RawModel *rawModel;

private:
  virtual void create() = 0;
  virtual void storeData() = 0;
  virtual void unbind() = 0;
};

template <class T> struct Data {
  T *data;
  int sizeOfData;
  int dataDimension;
};

class VAOLoader : public AbstractLoader {
public:
  VAOLoader(Data<float> vertexData, Data<float> textureCoords,
            Data<GLuint> indicesData, const int vertexNum, RawModel *rawModel);
  void cleanup() override;

private:
  void create() override;
  void storeData() override;
  void unbind() override;
  std::vector<GLuint> vaos;
  std::vector<GLuint> vbos;
  std::vector<GLuint> ebos;
  Data<float> vertexData;
  Data<float> textureCoords;
  Data<GLuint> indicesData;
  int vertexNum;
};

class TextureLoader : public AbstractLoader {
public:
  TextureLoader(std::string texturePath, RawModel *rawModel);
  void cleanup() override;

private:
  void create() override;
  void storeData() override;
  void unbind() override;
  std::string texturePath;
  std::vector<GLuint> textures;
};

#endif // OPENGL_LOADER_H
