#ifndef VK_BUFFERS_MANAGER_H
#define VK_BUFFERS_MANAGER_H

#include <map>

#include "vk_staging_buffer_factory.h"
#include "vk_vertex_buffer_factory.h"
#include "../vk_context_user.h"
#include "vk_buffer_wrapper.h"
#include "vk_mapped_buffer_wrapper.h"
#include "vk_index_buffer_factory.h"
#include "vk_uniform_buffer_factory.h"

class VkBuffersManager : public VkContextUser {
public:
  void Init();
  void Destroy();

  std::shared_ptr<VkBufferWrapper> GetVertexBufferWrapper(VkDeviceSize size);

  std::shared_ptr<VkBufferWrapper> CreateUniqueStagingBufferWrapper(VkDeviceSize size);
  void DestroyUniqueStagingBufferWrapper(const std::shared_ptr<VkBufferWrapper>& buffer);

  std::shared_ptr<VkBufferWrapper> GetStagingBufferWrapper(VkDeviceSize size);

  std::shared_ptr<VkBufferWrapper> GetIndexBufferWrapper(VkDeviceSize size);
  std::shared_ptr<VkMappedBufferWrapper> GetUniformBufferWrapper(int frame);


protected:
  void CreateUniformBuffers(int frames);
  void DestroyUniformBuffers();

protected:
  std::shared_ptr<VkBufferWrapper> vertexBufferCache;

  using TUniformBufferCache = std::vector<std::shared_ptr<VkMappedBufferWrapper>>;
  TUniformBufferCache uniformBufferCache;

  using TBufferCache = std::map<VkDeviceSize, std::shared_ptr<VkBufferWrapper>>;
  TBufferCache stagingBufferCache;

  std::shared_ptr<VkBufferWrapper> indexBufferCache;

  VkStagingBufferFactory stagingBufferFactory;
  VkVertexBufferFactory vertexBufferFactory;
  VkIndexBufferFactory indexBufferFactory;
  VkUniformBufferFactory uniformBufferFactory;
};



#endif //VK_BUFFERS_MANAGER_H
