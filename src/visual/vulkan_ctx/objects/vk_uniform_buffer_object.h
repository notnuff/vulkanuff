#ifndef VK_UNIFORM_BUFFER_OBJECT_H
#define VK_UNIFORM_BUFFER_OBJECT_H

#include <glm/glm.hpp>

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};



#endif //VK_UNIFORM_BUFFER_OBJECT_H
