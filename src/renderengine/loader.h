
#ifndef OPENGL_LOADER_H
#define OPENGL_LOADER_H

#include "../model/Scene.h"
#include "../model/model.h"
#include <glad/glad.h>
#include <string>
#include <vector>

class AbstractLoader {
public:
  AbstractLoader(Scene *scene) : scene(scene){};
  virtual void load() {
    create();
    storeData();
    unbind();
  };
  virtual void cleanup() = 0;
  Scene *scene;

private:
  virtual void create() = 0;
  virtual void storeData() = 0;
  virtual void unbind() = 0;
};

class VAOLoader : public AbstractLoader {
public:
  VAOLoader(Scene *scene);
  void cleanup() override;

private:
  void create() override;
  void storeData() override;
  void unbind() override;
  std::vector<GLuint> vaos;
  std::vector<GLuint> vbos;
  std::vector<GLuint> ebos;
};

class TextureLoader : public AbstractLoader {
public:
  TextureLoader(Scene *scene);
  void cleanup() override;
  void load() override;

private:
  void create() override;
  void storeData() override;
  void unbind() override;
  TextureData *textureData;
  std::vector<GLuint> textures;
};

#endif // OPENGL_LOADER_H
