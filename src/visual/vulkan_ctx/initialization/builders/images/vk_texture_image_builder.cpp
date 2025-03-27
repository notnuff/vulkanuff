#include "vk_texture_image_builder.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "buffers/vk_buffers_manager.h"

void VkTextureImageBuilder::CreateImage(uint32_t width, uint32_t height,
    VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
    VkMemoryPropertyFlags memProperties, VkImage& outImage,
    VkDeviceMemory& outImageMemory) {


  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width;
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;

  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;

  imageInfo.format = format;
  imageInfo.tiling = tiling;

  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.flags = 0; // Optional

  if (vkCreateImage(pCtx->device, &imageInfo, nullptr, &outImage) != VK_SUCCESS) {
    throw std::runtime_error("failed to create image!");
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(pCtx->device, outImage, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, memProperties);

  if (vkAllocateMemory(pCtx->device, &allocInfo, nullptr, &outImageMemory) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate image memory!");
  }

  vkBindImageMemory(pCtx->device, outImage, outImageMemory, 0);
}

void VkTextureImageBuilder::DoBuild() {
  int texWidth, texHeight, texChannels;
  stbi_uc* pixels = stbi_load("../textures/img.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  VkDeviceSize imageSize = texWidth * texHeight * 32/8;

  if (!pixels) {
    throw std::runtime_error("failed to load texture image!");
  }

  imageStagingBuffer = pCtx->pBuffersManager->CreateUniqueStagingBufferWrapper(imageSize);

  void* data;
  vkMapMemory(pCtx->device, imageStagingBuffer->Memory, 0, imageSize, 0, &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(pCtx->device, imageStagingBuffer->Memory);

  stbi_image_free(pixels);

  CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    pCtx->textureImage, pCtx->textureImageMemory);


  pCtx->PerformImageLayoutTransition(pCtx->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  pCtx->CopyBufferToImage(imageStagingBuffer->Buffer, pCtx->textureImage, texWidth, texHeight);
  pCtx->PerformImageLayoutTransition(pCtx->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

}

void VkTextureImageBuilder::DoDestroy() {
  pCtx->pBuffersManager->DestroyUniqueStagingBufferWrapper(imageStagingBuffer);

  vkDestroyImage(pCtx->device, pCtx->textureImage, nullptr);
  vkFreeMemory(pCtx->device, pCtx->textureImageMemory, nullptr);
}

// TODO: get rid of this copypaste
uint32_t VkTextureImageBuilder::FindMemoryType(uint32_t typeFilter,
    VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(pCtx->physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
}