#ifndef VK_DESCRIPTOR_POOL_BUILDER_H
#define VK_DESCRIPTOR_POOL_BUILDER_H


#include "../vk_ctx_builder_i.h"

class VkDescriptorPoolBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_DESCRIPTOR_POOL_BUILDER_H
