#ifndef VK_TEXTURE_IMAGE_BUILDER_H
#define VK_TEXTURE_IMAGE_BUILDER_H

#include "../vk_ctx_builder_i.h"

class VkTextureImageBuilder : public VkCtxBuilderI  {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_TEXTURE_IMAGE_BUILDER_H
