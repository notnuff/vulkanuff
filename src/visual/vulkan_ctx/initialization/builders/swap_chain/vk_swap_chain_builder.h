#ifndef VK_SWAP_CHAIN_BUILDER_H
#define VK_SWAP_CHAIN_BUILDER_H

#include <vulkan/vulkan_core.h>

#include <vector>

#include "../vk_ctx_builder_i.h"

class VkSwapChainBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

protected:
  VkSurfaceFormatKHR ChooseBestSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats);

  VkPresentModeKHR ChooseBestSwapSurfacePresentMode(
    const std::vector<VkPresentModeKHR>& availableModes);

  VkExtent2D ChooseSwapExtent(
      const VkSurfaceCapabilitiesKHR& capabilities);
};



#endif //VK_SWAP_CHAIN_BUILDER_H
