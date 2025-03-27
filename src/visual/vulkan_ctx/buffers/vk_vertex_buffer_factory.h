#ifndef VK_VERTEX_BUFFER_BUILDER_H
#define VK_VERTEX_BUFFER_BUILDER_H

#include "vk_buffer_creation_helper.h"
#include "vk_buffer_wrapper.h"

#include "initialization/builders/vk_ctx_builder_i.h"


class VkVertexBufferFactory : public VkBufferFactoryI {
public:
  virtual ~VkVertexBufferFactory() = default;

public:
  virtual std::shared_ptr<VkBufferWrapper> CreateBuffer(VkDeviceSize size) override;

};



#endif //VK_VERTEX_BUFFER_BUILDER_H
