#include "vk_buffers_builder.h"

#include <oneapi/tbb/detail/_task.h>

#include "../../../buffers/vk_buffers_manager.h"

void VkBuffersBuilder::DoBuild() {
  auto buffersManager = std::make_shared<VkBuffersManager>();
  buffersManager->Context(pCtx);
  buffersManager->Init();

  pCtx->pBuffersManager = buffersManager;
}

void VkBuffersBuilder::DoDestroy() {
  pCtx->pBuffersManager->Destroy();
}