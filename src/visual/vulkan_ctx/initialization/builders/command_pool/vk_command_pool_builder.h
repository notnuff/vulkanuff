#ifndef VK_COMMAND_POOL_BUILDER_H
#define VK_COMMAND_POOL_BUILDER_H

#include "initialization/builders/vk_ctx_builder_i.h"

class VkCommandPoolBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;
};



#endif //VK_COMMAND_POOL_BUILDER_H
