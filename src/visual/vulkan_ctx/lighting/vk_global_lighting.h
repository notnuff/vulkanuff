#ifndef VK_GLOBAL_LIGHTING_H
#define VK_GLOBAL_LIGHTING_H

#include <glm/glm.hpp>
struct GlobalLightingInfo {
  alignas(16) glm::vec3 direction;
  alignas(16) glm::vec4 color;
};


#endif //VK_GLOBAL_LIGHTING_H
