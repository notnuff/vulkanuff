#ifndef VK_IMAGE_VIEWS_BUILDER_H
#define VK_IMAGE_VIEWS_BUILDER_H

#include "../vk_ctx_builder_i.h"

class VkImageViewsBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_IMAGE_VIEWS_BUILDER_H
