#include "vk_buffers_manager.h"

void VkBuffersManager::Init() {
  stagingBufferFactory.Context(pCtx);
  vertexBufferFactory.Context(pCtx);
}

std::shared_ptr<VkBufferWrapper> VkBuffersManager::GetVertexBufferWrapper(VkDeviceSize size) {
  if (size == 0) throw std::invalid_argument("Invalid size");

  if (!vertexBufferCache) vertexBufferCache = vertexBufferFactory.CreateBuffer(size);

  if (vertexBufferCache->Size != size) {
    vertexBufferFactory.DestroyBuffer(*vertexBufferCache);
    vertexBufferCache = vertexBufferFactory.CreateBuffer(size);
  }

  return vertexBufferCache;
}

std::shared_ptr<VkBufferWrapper> VkBuffersManager::GetStagingBufferWrapper(VkDeviceSize size) {
  if (size == 0) throw std::invalid_argument("Invalid size");

  if (!stagingBufferCache) stagingBufferCache = stagingBufferFactory.CreateBuffer(size);

  if (stagingBufferCache->Size != size) {
    stagingBufferFactory.DestroyBuffer(*stagingBufferCache);
    stagingBufferCache = stagingBufferFactory.CreateBuffer(size);
  }

  return stagingBufferCache;
}