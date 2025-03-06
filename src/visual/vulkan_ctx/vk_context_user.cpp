//
// Created by notnuff on 06.03.25.
//

#include "vk_context_user.h"

VkContext* VkContextUser::Context() {
  return pCtx;
}

void VkContextUser::Context(VkContext* contextPtr) {
  pCtx = contextPtr;
}