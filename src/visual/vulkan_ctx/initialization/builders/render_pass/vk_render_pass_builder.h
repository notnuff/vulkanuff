#ifndef VK_RENDER_PASS_BUILDER_H
#define VK_RENDER_PASS_BUILDER_H

#include "../vk_ctx_builder_i.h"

class VkRenderPassBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;
};



#endif //VK_RENDER_PASS_BUILDER_H
