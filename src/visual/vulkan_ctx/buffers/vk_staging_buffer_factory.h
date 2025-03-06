#ifndef VK_STAGING_BUFFER_BUILDER_H
#define VK_STAGING_BUFFER_BUILDER_H

#include "vk_buffer_creation_helper.h"

class VkStagingBufferFactory : public VkBufferFactoryI {
public:
  virtual ~VkStagingBufferFactory() = default;

public:
  virtual std::shared_ptr<VkBufferWrapper> CreateBuffer(VkDeviceSize size) override;

};


#endif //VK_STAGING_BUFFER_BUILDER_H