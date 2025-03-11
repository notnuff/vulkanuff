#include "vk_texture_image_builder.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../../../buffers/vk_buffers_manager.h"

void VkTextureImageBuilder::DoBuild() {
  int texWidth, texHeight, texChannels;
  stbi_uc* pixels = stbi_load("../textures/img.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  VkDeviceSize imageSize = texWidth * texHeight * 32/8;

  if (!pixels) {
    throw std::runtime_error("failed to load texture image!");
  }

  // TODO: named buffers? what I have now is unsafe.
  const auto& imageStagingBuffer =
    pCtx->pBuffersManager->CreateUniqueStagingBufferWrapper(imageSize);

  void* data;
  vkMapMemory(pCtx->device, imageStagingBuffer->Memory, 0, imageSize, 0, &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(pCtx->device, imageStagingBuffer->Memory);

  stbi_image_free(pixels);
}

void VkTextureImageBuilder::DoDestroy() {

}