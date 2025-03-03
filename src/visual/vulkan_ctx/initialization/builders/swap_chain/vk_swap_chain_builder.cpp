#include "vk_swap_chain_builder.h"

#include <bits/stl_algo.h>
#include <vulkan/vulkan_core.h>

#include <limits>
#include <stdexcept>

#include "../../../common/vk_queue_family_indices.h"
#include "../../../common/vk_swap_chain_support_details.h"

void VkSwapChainBuilder::DoBuild() {
  SwapChainSupportDetails swapChainSupport =
      SwapChainSupportDetails::GetQuerySwapChainSupport(
          pCtx->physicalDevice, pCtx);

  VkSurfaceFormatKHR surfaceFormat =
      ChooseBestSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      ChooseBestSwapSurfacePresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = pCtx->surface;

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;

  // usage to draw directly onto the screen.
  // if we want to use post-process,
  // we will need to use VK_IMAGE_USAGE_TRANSFER_DST_BIT
  // and load the image later with memory operation

  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = QueueFamilyIndices::GetQueueFamilies(
      pCtx->physicalDevice, pCtx);
  uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                   indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;      // Optional
    createInfo.pQueueFamilyIndices = nullptr;  // Optional
  }

  // TODO: try to use VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

  // TODO: wait, i can make transparent windows?
  // uhm, i need to ask for this functionality first
  createInfo.compositeAlpha =
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;  // VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;

  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(pCtx->device, &createInfo, nullptr, &pCtx->swapChain) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain");
  }

  vkGetSwapchainImagesKHR(pCtx->device, pCtx->swapChain, &imageCount, nullptr);
  pCtx->swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(pCtx->device, pCtx->swapChain, &imageCount,
                          pCtx->swapChainImages.data());

  pCtx->swapChainImageFormat = surfaceFormat.format;
  pCtx->swapChainExtent = extent;
}

void VkSwapChainBuilder::DoDestroy() {
  for (auto framebuffer : pCtx->swapChainFramebuffers) {
    vkDestroyFramebuffer(pCtx->device, framebuffer, nullptr);
  }
  for (auto imageView : pCtx->swapChainImageViews) {
    vkDestroyImageView(pCtx->device, imageView, nullptr);
  }

  vkDestroySwapchainKHR(pCtx->device, pCtx->swapChain, nullptr);

}

VkSurfaceFormatKHR VkSwapChainBuilder::ChooseBestSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats) {
  for (const auto& availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  throw std::runtime_error(
      "damn bitch, you're better get normal format for swap chain");
}
VkPresentModeKHR VkSwapChainBuilder::ChooseBestSwapSurfacePresentMode(
    const std::vector<VkPresentModeKHR>& availableModes) {
  for (const auto& availablePresentMode : availableModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VkSwapChainBuilder::ChooseSwapExtent(
    const VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(pCtx->window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
  }
}