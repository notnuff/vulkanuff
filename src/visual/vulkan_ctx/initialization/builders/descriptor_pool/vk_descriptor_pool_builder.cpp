#include "vk_descriptor_pool_builder.h"

#include "../shared/vk_frames_in_flight.h"


void VkDescriptorPoolBuilder::DoBuild() {
  VkDescriptorPoolSize poolSize{};
  poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 1;
  poolInfo.pPoolSizes = &poolSize;

  poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

  if (vkCreateDescriptorPool(pCtx->device, &poolInfo, nullptr, &pCtx->descriptorPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }

}

void VkDescriptorPoolBuilder::DoDestroy() {
  vkDestroyDescriptorPool(pCtx->device, pCtx->descriptorPool, nullptr);
}