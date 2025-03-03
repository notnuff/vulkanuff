#ifndef VK_CTX_BUILDER_I_H
#define VK_CTX_BUILDER_I_H

#include "../../vk_context.h"

class VkContext;
class VkCtxBuilderI {
 public:
  VkCtxBuilderI() = default;
  virtual ~VkCtxBuilderI() = default;

 public:
  void SetCtx(VkContext* targetCtx);
  void Build();
  void Destroy();

 protected:
  virtual void DoBuild() = 0;
  virtual void DoDestroy() = 0;

 protected:
  VkContext* pCtx = nullptr;

};

#endif  // VK_CTX_BUILDER_I_H
