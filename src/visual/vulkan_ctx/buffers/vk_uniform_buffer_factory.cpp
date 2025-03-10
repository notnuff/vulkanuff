#include "vk_uniform_buffer_factory.h"

std::shared_ptr<VkBufferWrapper> VkUniformBufferFactory::CreateBuffer(
    VkDeviceSize size) {
  auto result = std::make_shared<VkBufferWrapper>();
  result->Size = size;

  VkBufferFactoryI::CreateBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               result->Buffer, result->Memory);

  return result;
}