#ifndef VK_VERTEX_BUFFER_BUILDER_H
#define VK_VERTEX_BUFFER_BUILDER_H
#include "../vk_ctx_builder_i.h"


class VkVertexBufferBuilder : public VkCtxBuilderI {
public:
  VkBufferCreateInfo& GetVkBufferCreateInfo();

protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

protected:
  uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

  VkBufferCreateInfo bufferInfo{};
};



#endif //VK_VERTEX_BUFFER_BUILDER_H
