#ifndef VK_FRAMEBUFFERS_BUILDER_H
#define VK_FRAMEBUFFERS_BUILDER_H

#include "../vk_ctx_builder_i.h"

class VkFramebuffersBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_FRAMEBUFFERS_BUILDER_H
