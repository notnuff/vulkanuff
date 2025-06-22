#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>

#include "vk_ambient_lighting.h"
#include "vk_global_lighting.h"
#include "vk_point_lighting.h"

struct LightingBufferObject {
  GlobalLightingInfo global;
  AmbientLightingInfo ambient;
  PointLightInfo point[4];
};



#endif //LIGHTING_H
