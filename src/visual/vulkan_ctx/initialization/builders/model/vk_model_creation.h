#ifndef VK_MODEL_CREATION_H
#define VK_MODEL_CREATION_H


#include "initialization/builders/vk_ctx_builder_i.h"

class VkModelCreation : public VkCtxBuilderI {
public:

protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_MODEL_CREATION_H
