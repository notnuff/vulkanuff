#ifndef VK_DESCRIPTOR_SET_LAYOUT_BUILDER_H
#define VK_DESCRIPTOR_SET_LAYOUT_BUILDER_H


#include "initialization/builders/vk_ctx_builder_i.h"

class VkDescriptorSetLayoutBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_DESCRIPTOR_SET_LAYOUT_BUILDER_H
