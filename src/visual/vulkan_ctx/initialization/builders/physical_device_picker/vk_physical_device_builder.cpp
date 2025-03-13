#include "vk_physical_device_builder.h"

#include <vulkan/vulkan_core.h>

#include <cstring>
#include <set>
#include <stdexcept>

#include "../../../common/vk_queue_family_indices.h"
#include "../../../common/vk_swap_chain_support_details.h"
#include "../../../vk_context.h"
#include "../shared/vk_physical_device_defines.h"

void VkPhysicalDeviceBuilder::DoBuild() {
  PickPhysicalDevice();
}

void VkPhysicalDeviceBuilder::DoDestroy() {}

void VkPhysicalDeviceBuilder::PickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(pCtx->instance, &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(pCtx->instance, &deviceCount,
                             devices.data());

  for (const auto& device : devices) {
    if (IsPhysicalDeviceSuitable(device)) {
      pCtx->physicalDevice = device;
      break;
    }
  }

  if (pCtx->physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

bool VkPhysicalDeviceBuilder::IsPhysicalDeviceSuitable(
    VkPhysicalDevice device) {
  auto indices = QueueFamilyIndices::GetQueueFamilies(device, pCtx);

  bool extensionsSupported = CheckDeviceExtensionsSupport(device);
  bool swapChainAdequate = false;

  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        SwapChainSupportDetails::GetQuerySwapChainSupport(device, pCtx);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  VkPhysicalDeviceFeatures supportedFeatures;
  vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

  return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool VkPhysicalDeviceBuilder::CheckDeviceExtensionsSupport(
    VkPhysicalDevice device) {
  // check swap chain support
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(sDeviceExtensions.begin(),
                                           sDeviceExtensions.end());

  for (const auto& extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty() && CheckSwapChainSupport(device);

}

bool VkPhysicalDeviceBuilder::CheckSwapChainSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());


  for (const auto& extension : availableExtensions) {
    if ( std::strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) != 0 ) continue;

    return true;
  }

  return false;
}
