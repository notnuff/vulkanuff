#ifndef VK_UNIFORM_BUFFER_FACTORY_H
#define VK_UNIFORM_BUFFER_FACTORY_H

#include "vk_buffer_creation_helper.h"

class VkUniformBufferFactory : public VkBufferFactoryI {
public:
  virtual ~VkUniformBufferFactory() = default;

public:
  virtual std::shared_ptr<VkBufferWrapper> CreateBuffer(VkDeviceSize size) override;

};



#endif //VK_UNIFORM_BUFFER_FACTORY_H
