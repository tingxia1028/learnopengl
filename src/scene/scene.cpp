
#include "scene.h"
#include "model.h"
#include <map>
Scene::Scene(std::vector<Model> &models, Camera *camera,
             std::vector<Light *> &lights, SkyBox *skyBox)
    : models(models), camera(camera), lights(lights), skyBox(skyBox) {}

void Scene::cleanUp() {
  for (unsigned int i = 0; i < models.size(); ++i) {
    std::map<std::string, TextureData> maps = models[i].loadedTextures;
    for (std::map<std::string, TextureData>::iterator it = maps.begin();
         it != maps.end(); ++it) {
      glDeleteTextures(1, &it->second.textureID);
    }
    for (Mesh &mesh : models[i].meshes) {
      glDeleteVertexArrays(1, &mesh.VAO);
      glDeleteBuffers(1, &mesh.VBO);
      glDeleteBuffers(1, &mesh.EBO);
    }
  }
}
