#ifndef VK_SWAP_CHAIN_SUPPORT_DETAILS_H_
#define VK_SWAP_CHAIN_SUPPORT_DETAILS_H_

#include <vulkan/vulkan_core.h>

#include <vector>

#include "vk_context.h"

struct SwapChainSupportDetails {
 public:
  VkSurfaceCapabilitiesKHR capabilities;

  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;

 public:
  static SwapChainSupportDetails GetQuerySwapChainSupport(
      VkPhysicalDevice device, VkContext *ctx);

};

#endif  // VK_SWAP_CHAIN_SUPPORT_DETAILS_H_
