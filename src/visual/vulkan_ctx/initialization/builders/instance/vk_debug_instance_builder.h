#ifndef VK_DEBUG_INSTANCE_BUILDER_H
#define VK_DEBUG_INSTANCE_BUILDER_H

#include <vector>

#include "initialization/builders/vk_ctx_builder_i.h"
#include "vk_instance_builder.h"

class VkContext;

class VkDebugInstanceBuilder : public VkInstanceBuilder {
 protected:
  virtual void DoBuild() override;

 protected:
  virtual std::vector<const char*> GetRequiredExtensions() override;

  void CheckValidationLayersSupport();
  bool IsValidationLayerAvailable(const char* layerName);

};

#endif  // VK_DEBUG_INSTANCE_BUILDER_H
