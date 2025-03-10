#include "vk_buffers_manager.h"

#include <ranges>

#include "../initialization/builders/shared/vk_frames_in_flight.h"

void VkBuffersManager::Init() {
  stagingBufferFactory.Context(pCtx);
  vertexBufferFactory.Context(pCtx);
  indexBufferFactory.Context(pCtx);

  CreateUniformBuffers(MAX_FRAMES_IN_FLIGHT);
}

void VkBuffersManager::Destroy() {
  for (const auto& cache : stagingBufferCache | std::views::values) {
    stagingBufferFactory.DestroyBuffer(*cache);
  }
  vertexBufferFactory.DestroyBuffer(*vertexBufferCache);
  indexBufferFactory.DestroyBuffer(*indexBufferCache);

  DestroyUniformBuffers();
}

void VkBuffersManager::CreateUniformBuffers(int frames) {
  uniformBufferFactory.Context(pCtx);

  uniformBufferCache.resize(frames);

  auto size = sizeof(VkMappedBufferWrapper);
  for(auto& cache : uniformBufferCache) {
    cache = std::make_shared<VkMappedBufferWrapper>();
    cache->BufferWrapper = uniformBufferFactory.CreateBuffer(size);
    vkMapMemory(pCtx->device, cache->BufferWrapper->Memory, 0, size, 0, &cache->MappedMemory);
  }
}

void VkBuffersManager::DestroyUniformBuffers() {
  for(auto& cache : uniformBufferCache) {
    vkUnmapMemory(pCtx->device, cache->BufferWrapper->Memory);
    uniformBufferFactory.DestroyBuffer(*cache->BufferWrapper);
  }
  uniformBufferCache.clear();
}

std::shared_ptr<VkBufferWrapper> VkBuffersManager::GetStagingBufferWrapper(
    VkDeviceSize size) {
  if (size == 0) throw std::invalid_argument("Invalid size");

  if (!stagingBufferCache.contains(size)) {
    stagingBufferCache[size] = stagingBufferFactory.CreateBuffer(size);
  }

  return stagingBufferCache[size];
}


std::shared_ptr<VkMappedBufferWrapper> VkBuffersManager::GetUniformBufferWrapper(int frame) {
  if (frame < 0 || frame >= uniformBufferCache.size() ) throw std::invalid_argument("invalid frame");

  return uniformBufferCache[frame];
};

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
GetBufferWrapper(index, Index)


#undef GetBufferWrapper