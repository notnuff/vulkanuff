#ifndef VK_INSTANCE_BUILDER_H
#define VK_INSTANCE_BUILDER_H

#include <vulkan/vulkan_core.h>

#include <vector>

#include "../vk_ctx_builder_i.h"

class VkContext;

class VkInstanceBuilder : public VkCtxBuilderI {
 public:
  VkInstanceBuilder();

 protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

  virtual std::vector<const char*> GetRequiredExtensions();


 protected:
  VkApplicationInfo appInfo = {};
  VkInstanceCreateInfo createInfo = {};
};

#endif  // VK_INSTANCE_BUILDER_H
