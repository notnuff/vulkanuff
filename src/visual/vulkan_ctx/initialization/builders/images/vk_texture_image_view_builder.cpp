#include "vk_texture_image_view_builder.h"
#include <vulkan/vulkan_core.h>

#include <stdexcept>

#include "../../../vk_context.h"


void VkTextureImageViewBuilder::DoBuild() {
  // TODO: remove copypaste from swap chain image views creation
  VkImageViewCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

  createInfo.image = pCtx->textureImage;

  createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;

  createInfo.components = {
    VK_COMPONENT_SWIZZLE_IDENTITY,
    VK_COMPONENT_SWIZZLE_IDENTITY,
    VK_COMPONENT_SWIZZLE_IDENTITY,
    VK_COMPONENT_SWIZZLE_IDENTITY,
};

  createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;

  if(vkCreateImageView(pCtx->device, &createInfo, nullptr, &pCtx->textureImageView) != VK_SUCCESS) {
    throw std::runtime_error("failed to create texture image view!");
  }

}

void VkTextureImageViewBuilder::DoDestroy() {
  vkDestroyImageView(pCtx->device, pCtx->textureImageView, nullptr);
}