#include "vk_buffers_manager.h"

#include <ranges>

void VkBuffersManager::Init() {
  stagingBufferFactory.Context(pCtx);
  vertexBufferFactory.Context(pCtx);
  indexBufferFactory.Context(pCtx);
}

void VkBuffersManager::Destroy() {
  for (const auto& cache : stagingBufferCache | std::views::values) {
    stagingBufferFactory.DestroyBuffer(*cache);
  }
  vertexBufferFactory.DestroyBuffer(*vertexBufferCache);
  indexBufferFactory.DestroyBuffer(*indexBufferCache);
}

std::shared_ptr<VkBufferWrapper> VkBuffersManager::GetStagingBufferWrapper(
    VkDeviceSize size) {
  if (size == 0) throw std::invalid_argument("Invalid size");

  if (!stagingBufferCache.contains(size)) {
    stagingBufferCache[size] = stagingBufferFactory.CreateBuffer(size);
  }

  return stagingBufferCache[size];
}

#define GetBufferWrapper(NAME, FUNC_NAME) \
std::shared_ptr<VkBufferWrapper> VkBuffersManager::Get##FUNC_NAME##BufferWrapper(VkDeviceSize size) { \
  if (size == 0) throw std::invalid_argument("Invalid size"); \
\
  if (!NAME##BufferCache) NAME##BufferCache = NAME##BufferFactory.CreateBuffer(size); \
\
  if (NAME##BufferCache->Size != size) { \
    NAME##BufferFactory.DestroyBuffer(*NAME##BufferCache); \
    NAME##BufferCache = NAME##BufferFactory.CreateBuffer(size); \
  } \
 \
  return NAME##BufferCache; \
}

GetBufferWrapper(vertex, Vertex);
// GetBufferWrapper(staging, Staging);
GetBufferWrapper(index, Index);

#undef GetBufferWrapper