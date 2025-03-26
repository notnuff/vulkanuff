#include "vk_depth_buffer_resources_builder.h"

#include "../../vk_ctx_build_director.h"
#include "../images/vk_texture_image_builder.h"


void VkDepthBufferResourcesBuilder::DoBuild() {
  VkFormat depthFormat = GetDepthFormat();
  const auto& imageBuilder = pCtx->pCreator->GetBuilderByType<VkTextureImageBuilder>();
  imageBuilder->CreateImage(
    pCtx->swapChainExtent.width, pCtx->swapChainExtent.height,
    depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    pCtx->depthImage, pCtx->depthImageMemory
  );

  // TODO: remove copypaste from swap chain image views creation
  VkImageViewCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

  createInfo.image = pCtx->depthImage;

  createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  createInfo.format = depthFormat;

  createInfo.components = {
    VK_COMPONENT_SWIZZLE_IDENTITY,
    VK_COMPONENT_SWIZZLE_IDENTITY,
    VK_COMPONENT_SWIZZLE_IDENTITY,
    VK_COMPONENT_SWIZZLE_IDENTITY,
};

  createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;

  if(vkCreateImageView(pCtx->device, &createInfo, nullptr, &pCtx->depthImageView) != VK_SUCCESS) {
    throw std::runtime_error("failed to create texture image view!");
  }

  pCtx->PerformImageLayoutTransition(pCtx->depthImage, createInfo.format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

}

void VkDepthBufferResourcesBuilder::DoDestroy() {}

VkFormat VkDepthBufferResourcesBuilder::GetDepthFormat() {
  return GetAvailableFormat(
    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
    VK_IMAGE_TILING_OPTIMAL,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
  );


}

VkFormat VkDepthBufferResourcesBuilder::GetAvailableFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(pCtx->physicalDevice, format, &props);
    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
      return format;
    }
    if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  throw std::runtime_error("failed to find supported format!");
}

bool VkDepthBufferResourcesBuilder::IsFormatHasStencil(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

