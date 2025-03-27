#include "initialization/builders/vk_ctx_builder_i.h"

#include <stdexcept>

#include "vk_context.h"

void VkCtxBuilderI::Build() {
  if (!pCtx) throw std::runtime_error("pTargetContext is null");

  DoBuild();
}

void VkCtxBuilderI::Destroy() {
  if (!pCtx) throw std::runtime_error("pTargetContext is null");

  DoDestroy();
}
