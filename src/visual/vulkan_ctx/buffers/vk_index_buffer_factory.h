#ifndef VK_INDEX_BUFFER_FACTORY_H
#define VK_INDEX_BUFFER_FACTORY_H

#include "vk_buffer_creation_helper.h"

class VkIndexBufferFactory : public VkBufferFactoryI {
public:
  virtual ~VkIndexBufferFactory() = default;

public:
  virtual std::shared_ptr<VkBufferWrapper> CreateBuffer(VkDeviceSize size) override;

};



#endif //VK_INDEX_BUFFER_FACTORY_H
