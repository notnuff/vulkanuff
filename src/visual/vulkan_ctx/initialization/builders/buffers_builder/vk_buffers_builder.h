#ifndef VK_BUFFERS_BUILDER_H
#define VK_BUFFERS_BUILDER_H

#include "../vk_ctx_builder_i.h"

class VkBuffersBuilder : public VkCtxBuilderI {
protected:
  virtual void DoBuild() override;
  virtual void DoDestroy() override;

};



#endif //VK_BUFFERS_BUILDER_H
