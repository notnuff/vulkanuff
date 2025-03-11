#ifndef VK_UNIFORM_BUFFER_OBJECT_H
#define VK_UNIFORM_BUFFER_OBJECT_H

#include <glm/glm.hpp>

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};



#endif //VK_UNIFORM_BUFFER_OBJECT_H
