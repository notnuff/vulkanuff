#ifndef VK_PHYSICAL_DEVICE_BUILDER_H
#define VK_PHYSICAL_DEVICE_BUILDER_H

#include <vulkan/vulkan_core.h>

#include "../vk_ctx_builder_i.h"
#include "../../../vk_context.h"

class VkPhysicalDeviceBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

protected:
  void PickPhysicalDevice();
  bool IsPhysicalDeviceSuitable(VkPhysicalDevice device);
  bool CheckDeviceExtensionsSupport(VkPhysicalDevice device);
  bool CheckSwapChainSupport(VkPhysicalDevice device);

};



#endif //VK_PHYSICAL_DEVICE_BUILDER_H
