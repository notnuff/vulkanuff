#ifndef VK_DEPTH_BUFFER_BUILDER_H
#define VK_DEPTH_BUFFER_BUILDER_H

#include "initialization/builders/vk_ctx_builder_i.h"

class VkDepthBufferResourcesBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;


  VkFormat GetAvailableFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

public:
  VkFormat GetDepthFormat();
  bool IsFormatHasStencil(VkFormat format);

};



#endif //VK_DEPTH_BUFFER_BUILDER_H
