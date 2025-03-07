#include "vk_index_buffer_factory.h"

std::shared_ptr<VkBufferWrapper> VkIndexBufferFactory::CreateBuffer(
    VkDeviceSize size) {

  auto result = std::make_shared<VkBufferWrapper>();
  result->Size = size;

  VkBufferFactoryI::CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
               result->Buffer, result->Memory);

  return result;
}