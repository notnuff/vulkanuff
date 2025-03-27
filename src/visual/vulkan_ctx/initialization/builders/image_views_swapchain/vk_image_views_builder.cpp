#include "vk_image_views_builder.h"

#include <vulkan/vulkan_core.h>

#include <stdexcept>

#include "vk_context.h"

void VkImageViewsBuilder::DoBuild() {
  pCtx->swapChainImageViews.resize(
      pCtx->swapChainImages.size()
      );

  int i = 0;
  for (auto& img : pCtx->swapChainImages) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = img;

    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = pCtx->swapChainImageFormat;

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

    if (vkCreateImageView(pCtx->device, &createInfo, nullptr,
                          &pCtx->swapChainImageViews[i]) !=
        VK_SUCCESS) {
      throw std::runtime_error("failed to create image views!");
    }

    ++i;
  }
}

void VkImageViewsBuilder::DoDestroy() {
}