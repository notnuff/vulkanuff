#ifndef VK_SURFACE_BUILDER_H
#define VK_SURFACE_BUILDER_H

#include "../vk_ctx_builder_i.h"

class VkSurfaceBuilder : public VkCtxBuilderI {
 protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};

#endif  // VK_SURFACE_BUILDER_H
