#include "vk_vertex_buffer_factory.h"

#include <iostream>

#include "../common/vertex.h"


std::shared_ptr<VkBufferWrapper> VkVertexBufferFactory::CreateBuffer(
    VkDeviceSize size) {
  auto result = std::make_shared<VkBufferWrapper>();
  result->Size = size;

  VkBufferFactoryI::CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
               result->Buffer, result->Memory);

  return result;
}