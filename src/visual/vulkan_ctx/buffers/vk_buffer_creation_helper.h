#ifndef VK_BUFFER_CREATION_HELPER_H
#define VK_BUFFER_CREATION_HELPER_H


#include <memory>

#include "../vk_context_user.h"
#include "vk_buffer_wrapper.h"

class VkBufferFactoryI : public VkContextUser {
public:
  virtual std::shared_ptr<VkBufferWrapper> CreateBuffer(VkDeviceSize size) = 0;
  void DestroyBuffer(const VkBufferWrapper& bufferWrapper);

protected:
  void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                    VkBuffer& buffer, VkDeviceMemory& bufferMemory);

  uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

};



#endif //VK_BUFFER_CREATION_HELPER_H
