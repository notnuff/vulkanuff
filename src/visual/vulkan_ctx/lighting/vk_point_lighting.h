#ifndef VK_POINT_LIGHTING_H
#define VK_POINT_LIGHTING_H

#include <glm/glm.hpp>

struct PointLightInfo {
  glm::vec3 position;
  float radius;
  glm::vec4 color; // w is intensity
};

#endif //VK_POINT_LIGHTING_H