#ifndef VK_CTX_BUILDER_I_H
#define VK_CTX_BUILDER_I_H

#include "../../vk_context.h"
#include "../../vk_context_user.h"

class VkContext;
class VkCtxBuilderI : public VkContextUser {
 public:
  VkCtxBuilderI() = default;
  virtual ~VkCtxBuilderI() = default;

 public:
  void Build();
  void Destroy();

 protected:
  virtual void DoBuild() = 0;
  virtual void DoDestroy() = 0;
};

#endif  // VK_CTX_BUILDER_I_H
