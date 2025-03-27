#ifndef VK_TEXTURE_IMAGE_BUILDER_H
#define VK_TEXTURE_IMAGE_BUILDER_H

#include "initialization/builders/vk_ctx_builder_i.h"

class VkBufferWrapper;

class VkTextureImageBuilder : public VkCtxBuilderI  {
public:

  void CreateImage(uint32_t width, uint32_t height,
    VkFormat format, VkImageTiling tiling,
    VkImageUsageFlags usage, VkMemoryPropertyFlags memProperties,
    VkImage& outImage, VkDeviceMemory& outImageMemory);

protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

  uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

  std::shared_ptr<VkBufferWrapper> imageStagingBuffer;
};



#endif //VK_TEXTURE_IMAGE_BUILDER_H
