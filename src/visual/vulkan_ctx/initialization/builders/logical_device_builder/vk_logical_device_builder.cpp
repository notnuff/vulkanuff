#include "vk_logical_device_builder.h"

#include <vulkan/vulkan_core.h>

#include <stdexcept>
#include <vector>

#include "common/vk_queue_family_indices.h"
#include "vk_context.h"
#include "initialization/builders/shared/vk_physical_device_defines.h"

void VkLogicalDeviceBuilder::DoBuild() {
  QueueFamilyIndices indices = QueueFamilyIndices::GetQueueFamilies(
      pCtx->physicalDevice, pCtx);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  float queuePriority = 1.0f;

  {  // graphics queue
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    queueCreateInfos.push_back(queueCreateInfo);
  }

  // TODO: need to be fucking carefull with this shit,
  // idk yet if it will work
  if (indices.graphicsFamily.value() != indices.presentFamily.value()) {  // surface queue
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.presentFamily.value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());

  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(sDeviceExtensions.size());
  createInfo.ppEnabledExtensionNames = sDeviceExtensions.data();

  if (vkCreateDevice(pCtx->physicalDevice, &createInfo, nullptr,
                     &pCtx->device) != VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(pCtx->device, indices.graphicsFamily.value(), 0,
                   &pCtx->graphicsQueue);
  vkGetDeviceQueue(pCtx->device, indices.presentFamily.value(), 0,
                   &pCtx->presentQueue);
}

void VkLogicalDeviceBuilder::DoDestroy() {
  vkDestroyDevice(pCtx->device, nullptr);
}

void VkLogicalDeviceBuilder::CleanupSwapChain() {
  // vkDestroyDevice(device, nullptr);
}