#include "vk_command_buffers_builder.h"

#include <stdexcept>

#include "../../../vk_context.h"
#include "../shared/vk_frames_in_flight.h"
#include "../../../common/vk_queue_family_indices.h"
#include "../../../common/vk_swap_chain_support_details.h"



void VkCommandBuffersBuilder::DoBuild() {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = pCtx->commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

  pCtx->commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  if (vkAllocateCommandBuffers( pCtx->device, &allocInfo, pCtx->commandBuffers.data()) !=
    VK_SUCCESS) {
      throw std::runtime_error("failed to allocate command buffers!");
    }
}

void VkCommandBuffersBuilder::DoDestroy() {
  // don't need to do anything, command pool should destroy also command buffers
}