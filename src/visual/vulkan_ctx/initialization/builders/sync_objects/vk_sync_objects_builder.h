#ifndef VK_SYNC_OBJECTS_BUILDER_H
#define VK_SYNC_OBJECTS_BUILDER_H

#include "initialization/builders/vk_ctx_builder_i.h"

class VkSyncObjectsBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;
};



#endif //VK_SYNC_OBJECTS_BUILDER_H
