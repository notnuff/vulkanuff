#ifndef VK_QUEUE_FAMILY_INDICES_H
#define VK_QUEUE_FAMILY_INDICES_H

#include <vulkan/vulkan.h>

#include <optional>

class VkContext;
struct QueueFamilyIndices {
public:
  std::optional<uint32_t> graphicsFamily;

  // so we actually can have the device,
  // that can draw images (which is defined by existing of appropriate queue family frfr)
  // but can't present them to the surface (window)
  std::optional<uint32_t> presentFamily;

  bool IsComplete();

public:
  static QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device, VkContext* ctx);

};



#endif //VK_QUEUE_FAMILY_INDICES_H
