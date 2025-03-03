#include "vk_sync_objects_builder.h"

#include <vulkan/vulkan_core.h>

#include <stdexcept>

#include "../shared/vk_frames_in_flight.h"
#include "../../../common/vk_swap_chain_support_details.h"

void VkSyncObjectsBuilder::DoBuild() {
  pCtx->imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  pCtx->renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  pCtx->inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    if (vkCreateSemaphore(pCtx->device, &semaphoreInfo, nullptr,
                          &pCtx->imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(pCtx->device, &semaphoreInfo, nullptr,
                          &pCtx->renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(pCtx->device, &fenceInfo, nullptr, &pCtx->inFlightFences[i]) !=
            VK_SUCCESS) {
      throw std::runtime_error("failed to create sync objects!");
            }
  }
}

void VkSyncObjectsBuilder::DoDestroy() {
  for (const auto& sem : pCtx->imageAvailableSemaphores) {
    vkDestroySemaphore(pCtx->device, sem, nullptr);
  }
  pCtx->imageAvailableSemaphores.clear();

  for (const auto& sem : pCtx->renderFinishedSemaphores) {
    vkDestroySemaphore(pCtx->device, sem, nullptr);
  }
  pCtx->renderFinishedSemaphores.clear();

  for (const auto& fen : pCtx->inFlightFences) {
    vkDestroyFence(pCtx->device, fen, nullptr);
  }
  pCtx->inFlightFences.clear();
}