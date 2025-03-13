#ifndef VK_TEXTURE_IMAGE_VIEW_H
#define VK_TEXTURE_IMAGE_VIEW_H

#include "../vk_ctx_builder_i.h"

class VkTextureImageViewBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_TEXTURE_IMAGE_VIEW_H
