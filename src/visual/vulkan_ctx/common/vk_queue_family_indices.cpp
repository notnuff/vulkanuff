#include "vk_queue_family_indices.h"

#include <vector>
#include "../vk_context.h"

bool QueueFamilyIndices::IsComplete() {
  return graphicsFamily.has_value() && presentFamily.has_value();
}

QueueFamilyIndices QueueFamilyIndices::GetQueueFamilies(VkPhysicalDevice device,
                                                        VkContext* ctx) {

  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto& queueFamily : queueFamilies) {
    { // check if queue can draw anything
      if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.graphicsFamily = i;
      }
    }

    { // check if queue can present anything to the surface
      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, ctx->surface, &presentSupport);
      if (presentSupport) {
        indices.presentFamily = i;
      }
    }

    if ( indices.IsComplete() ) {
      break;
    }

    i++;
  }

  return indices;


}