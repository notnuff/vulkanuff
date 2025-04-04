#ifndef VK_TEXTURE_IMAGE_SAMPLER_BUILDER_H
#define VK_TEXTURE_IMAGE_SAMPLER_BUILDER_H

#include "initialization/builders/vk_ctx_builder_i.h"

class VkTextureImageSamplerBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_TEXTURE_IMAGE_SAMPLER_BUILDER_H
