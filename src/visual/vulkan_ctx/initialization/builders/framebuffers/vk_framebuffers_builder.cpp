#include "vk_framebuffers_builder.h"

#include <vulkan/vulkan_core.h>

#include <stdexcept>

#include "../../../vk_context.h"

void VkFramebuffersBuilder::DoBuild() {
  pCtx->swapChainFramebuffers.resize(pCtx->swapChainImageViews.size());

  for (size_t i = 0; i < pCtx->swapChainImageViews.size(); i++) {
    VkImageView attachments[] = {pCtx->swapChainImageViews[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = pCtx->renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = pCtx->swapChainExtent.width;
    framebufferInfo.height = pCtx->swapChainExtent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(pCtx->device, &framebufferInfo, nullptr,
                            &pCtx->swapChainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
                            }
  }
}

void VkFramebuffersBuilder::DoDestroy() {

}