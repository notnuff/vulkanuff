#include "vk_swap_chain_support_details.h"

#include "vk_context.h"

SwapChainSupportDetails SwapChainSupportDetails::
    GetQuerySwapChainSupport(
    VkPhysicalDevice device, VkContext* ctx) {


  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, ctx->surface,
                                            &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, ctx->surface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, ctx->surface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, ctx->surface, &presentModeCount,
                                            nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, ctx->surface, &presentModeCount, details.presentModes.data());
  }

  return details;

}
