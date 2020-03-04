
#include "flashlight.h"
FlashLight::FlashLight(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                       const glm::vec3 &specular, const LightType lightType,
                       float constTerm, float linearTerm, float quadraticTerm,
                       float cutoffCos, float outCutoffCos)
    : Light(ambient, diffuse, specular, lightType), constTerm(constTerm),
      linearTerm(linearTerm), quadraticTerm(quadraticTerm),
      cutoffCos(cutoffCos), outCutoffCos(outCutoffCos) {}
