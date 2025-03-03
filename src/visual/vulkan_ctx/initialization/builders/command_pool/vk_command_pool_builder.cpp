#include "vk_command_pool_builder.h"

#include <vulkan/vulkan_core.h>

#include <stdexcept>
#include "../../../common/vk_queue_family_indices.h"
#include "../../../common/vk_swap_chain_support_details.h"

void VkCommandPoolBuilder::DoBuild() {
  QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::GetQueueFamilies(pCtx->physicalDevice, pCtx);

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

  if (vkCreateCommandPool(pCtx->device, &poolInfo, nullptr, &pCtx->commandPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
      }
}

void VkCommandPoolBuilder::DoDestroy() {
  vkDestroyCommandPool(pCtx->device, pCtx->commandPool, nullptr);
}