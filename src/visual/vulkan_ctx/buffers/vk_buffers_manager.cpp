#include "vk_buffers_manager.h"

#include <ranges>

#include "initialization/builders/shared/vk_frames_in_flight.h"

// TODO: use pools

void VkBuffersManager::Init() {
  stagingBufferFactory.Context(pCtx);
  vertexBufferFactory.Context(pCtx);
  indexBufferFactory.Context(pCtx);

  CreateUniformBuffers(MAX_FRAMES_IN_FLIGHT);
  CreateLightingBuffers(MAX_FRAMES_IN_FLIGHT);
}

void VkBuffersManager::Destroy() {
  for (const auto& cache : stagingBufferCache | std::views::values) {
    stagingBufferFactory.DestroyBuffer(*cache);
  }
  vertexBufferFactory.DestroyBuffer(*vertexBufferCache);
  indexBufferFactory.DestroyBuffer(*indexBufferCache);

  DestroyUniformBuffers();
  DestroyLightingBuffers();
}

void VkBuffersManager::CreateUniformBuffers(int frames) {
  uniformBufferFactory.Context(pCtx);

  uniformBufferCache.resize(frames);

  auto size = sizeof(UniformBufferObject);
  for(auto& cache : uniformBufferCache) {
    cache = std::make_shared<VkMappedBufferWrapper>();
    cache->BufferWrapper = uniformBufferFactory.CreateBuffer(size);
    vkMapMemory(pCtx->device, cache->BufferWrapper->Memory, 0, size, 0, &cache->MappedMemory);
  }
}

void VkBuffersManager::CreateLightingBuffers(int frames) {
  uniformBufferFactory.Context(pCtx);

  lightingBufferCache.resize(frames);

  auto size = sizeof(LightingBufferObject);
  for(auto& cache : lightingBufferCache) {
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


void VkBuffersManager::DestroyLightingBuffers() {
  for(auto& cache : lightingBufferCache) {
    vkUnmapMemory(pCtx->device, cache->BufferWrapper->Memory);
    uniformBufferFactory.DestroyBuffer(*cache->BufferWrapper);
  }
  lightingBufferCache.clear();
}

std::shared_ptr<VkBufferWrapper> VkBuffersManager::GetStagingBufferWrapper(
    VkDeviceSize size) {
  if (size == 0) throw std::invalid_argument("Invalid size");

  if (!stagingBufferCache.contains(size)) {
    stagingBufferCache[size] = stagingBufferFactory.CreateBuffer(size);
  }

  return stagingBufferCache[size];
}


std::shared_ptr<VkBufferWrapper> VkBuffersManager::CreateUniqueStagingBufferWrapper(VkDeviceSize size) {
  return stagingBufferFactory.CreateBuffer(size);
}

void VkBuffersManager::DestroyUniqueStagingBufferWrapper(const std::shared_ptr<VkBufferWrapper>& buffer) {
  stagingBufferFactory.DestroyBuffer(*buffer);
};

std::shared_ptr<VkMappedBufferWrapper> VkBuffersManager::GetUniformBufferWrapper(int frame) {
  if (frame < 0 || frame >= uniformBufferCache.size() ) throw std::invalid_argument("invalid frame");

  return uniformBufferCache[frame];
}

std::shared_ptr<VkMappedBufferWrapper> VkBuffersManager::GetLightingBufferWrapper(int frame) {
  if (frame < 0 || frame >= lightingBufferCache.size() ) throw std::invalid_argument("invalid frame");

  return lightingBufferCache[frame];
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

GetBufferWrapper(vertex, Vertex)

GetBufferWrapper(index, Index)


#undef GetBufferWrapper