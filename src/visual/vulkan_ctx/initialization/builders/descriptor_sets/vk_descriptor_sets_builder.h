#ifndef VK_DESCRIPTOR_SETS_BUILDER_H
#define VK_DESCRIPTOR_SETS_BUILDER_H

#include "../vk_ctx_builder_i.h"


class VkDescriptorSetsBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_DESCRIPTOR_SETS_BUILDER_H
