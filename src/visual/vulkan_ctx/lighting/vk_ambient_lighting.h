#ifndef VK_AMBIENT_LIGHTING_H
#define VK_AMBIENT_LIGHTING_H

#include <glm/glm.hpp>
struct AmbientLightingInfo {
  alignas(16) glm::vec4 color; // w is intencity
};


#endif //VK_AMBIENT_LIGHTING_H
