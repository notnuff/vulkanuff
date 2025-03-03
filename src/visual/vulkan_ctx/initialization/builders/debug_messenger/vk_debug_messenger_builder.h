#ifndef VK_DEBUG_MESSENGER_BUILDER_H
#define VK_DEBUG_MESSENGER_BUILDER_H

#include <vulkan/vulkan_core.h>

#include <vector>

#include "../vk_ctx_builder_i.h"


class VkDebugMessengerBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

protected:
VkResult CreateDebugUtilsMessengerEXTProxy(VkInstance instance,
  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
  const VkAllocationCallbacks* pAllocator,
  VkDebugUtilsMessengerEXT* pDebugMessenger);

  void DestroyDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator);

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
  );

};



#endif //VK_DEBUG_MESSENGER_BUILDER_H
