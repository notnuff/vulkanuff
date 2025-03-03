#ifndef VK_LOGICAL_DEVICE_BUILDER_H_
#define VK_LOGICAL_DEVICE_BUILDER_H_

#include "../vk_ctx_builder_i.h"

class VkLogicalDeviceBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

protected:
  void CleanupSwapChain();

};

#endif  // VK_LOGICAL_DEVICE_BUILDER_H_
