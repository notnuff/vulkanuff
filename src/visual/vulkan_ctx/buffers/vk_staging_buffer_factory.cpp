#include "vk_staging_buffer_factory.h"

#include "common/vertex.h"


std::shared_ptr<VkBufferWrapper> VkStagingBufferFactory::CreateBuffer(
    VkDeviceSize size) {
  auto result = std::make_shared<VkBufferWrapper>();
  result->Size = size;

  VkBufferFactoryI::CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               result->Buffer, result->Memory);

  return result;
}