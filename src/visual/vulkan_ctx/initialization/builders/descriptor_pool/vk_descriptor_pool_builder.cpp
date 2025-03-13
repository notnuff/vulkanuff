#include "vk_descriptor_pool_builder.h"

#include <array>

#include "../shared/vk_frames_in_flight.h"


void VkDescriptorPoolBuilder::DoBuild() {
  VkDescriptorPoolSize poolSizeUniform{};
  poolSizeUniform.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizeUniform.descriptorCount = MAX_FRAMES_IN_FLIGHT;

  VkDescriptorPoolSize poolSizeSampler{};
  poolSizeSampler.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizeSampler.descriptorCount = MAX_FRAMES_IN_FLIGHT;

  std::array poolSizes = { poolSizeUniform, poolSizeSampler };

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = poolSizes.size();
  poolInfo.pPoolSizes = poolSizes.data();

  poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

  if (vkCreateDescriptorPool(pCtx->device, &poolInfo, nullptr, &pCtx->descriptorPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }

}

void VkDescriptorPoolBuilder::DoDestroy() {
  vkDestroyDescriptorPool(pCtx->device, pCtx->descriptorPool, nullptr);
}