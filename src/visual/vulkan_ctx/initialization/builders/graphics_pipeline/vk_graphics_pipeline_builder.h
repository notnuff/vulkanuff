#ifndef VK_GRAPHICS_PIPELINE_BUILDER_H
#define VK_GRAPHICS_PIPELINE_BUILDER_H

#include <vulkan/vulkan_core.h>

#include <vector>

#include "../vk_ctx_builder_i.h"
class VkGraphicsPipelineBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

protected:
  VkShaderModule CreateShaderModule(
    const std::vector<char>& code);
};



#endif //VK_GRAPHICS_PIPELINE_BUILDER_H
