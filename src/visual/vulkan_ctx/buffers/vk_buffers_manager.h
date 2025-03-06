#ifndef VK_BUFFERS_MANAGER_H
#define VK_BUFFERS_MANAGER_H

#include "vk_staging_buffer_factory.h"
#include "vk_vertex_buffer_factory.h"
#include "../vk_context_user.h"
#include "vk_buffer_wrapper.h"

class VkBuffersManager : public VkContextUser {
public:
  void Init();

  std::shared_ptr<VkBufferWrapper> GetVertexBufferWrapper(VkDeviceSize size);
  std::shared_ptr<VkBufferWrapper> GetStagingBufferWrapper(VkDeviceSize size);

protected:
  std::shared_ptr<VkBufferWrapper> vertexBufferCache;
  std::shared_ptr<VkBufferWrapper> stagingBufferCache;

  VkStagingBufferFactory stagingBufferFactory;
  VkVertexBufferFactory vertexBufferFactory;
};



#endif //VK_BUFFERS_MANAGER_H
