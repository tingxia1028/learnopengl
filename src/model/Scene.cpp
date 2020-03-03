
#include "Scene.h"
Scene::Scene(std::vector<Model> &models, Camera *camera,
             std::vector<Light> &lights)
    : models(models), camera(camera), lights(lights) {}
