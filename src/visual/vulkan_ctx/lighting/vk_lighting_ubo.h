#ifndef LIGHTING_H
#define LIGHTING_H
#include <glm/glm.hpp>

struct LightingBufferObject {
  alignas(16) glm::vec3 lightingDirection;
};



#endif //LIGHTING_H
