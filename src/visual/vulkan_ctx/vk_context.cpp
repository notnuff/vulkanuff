#include "vk_context.h"

#include <cstring>
#include <stdexcept>

#include "common/vertex.h"
#include "initialization/vk_ctx_build_director.h"
#include "initialization/builders/command_buffers/vk_command_buffers_builder.h"
#include "initialization/builders/framebuffers/vk_framebuffers_builder.h"
#include "initialization/builders/image_views/vk_image_views_builder.h"
#include "initialization/builders/shared/vk_frames_in_flight.h"
#include "initialization/builders/swap_chain/vk_swap_chain_builder.h"
#include "buffers/vk_vertex_buffer_factory.h"
#include "buffers/vk_buffers_manager.h"

void VkContext::DrawFrame() {
  void* data;
  auto buffersSize = sizeof(testVertices[0]) * testVertices.size();
  const auto& stagingBufferWrapper = pBuffersManager->GetStagingBufferWrapper(buffersSize);
  const auto& vertexBufferWrapper = pBuffersManager->GetVertexBufferWrapper(buffersSize);

  vkMapMemory(device, stagingBufferWrapper->Memory, 0, buffersSize, 0, &data);
  memcpy(data, testVertices.data(), (size_t) buffersSize);
  vkUnmapMemory(device, stagingBufferWrapper->Memory);

  CopyBuffer(stagingBufferWrapper->Buffer, vertexBufferWrapper->Buffer, buffersSize);

  vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    RecreateSwapChain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }
  vkResetFences(device, 1, &inFlightFences[currentFrame]);

  vkResetCommandBuffer(commandBuffers[currentFrame], 0);

  // TODO: maybe move RecordCommandBuffer from VkCommandBuffersBuilder somewhere else
  // pCreator->GetBuilderByType<VkCommandBuffersBuilder>()->RecordCommandBuffer(commandBuffers[currentFrame], imageIndex);

  RecordCommandBuffer(commandBuffers[currentFrame], imageIndex);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

  VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame] ) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;

  presentInfo.pResults = nullptr; // Optional

  result = vkQueuePresentKHR(presentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
    RecreateSwapChain();
    framebufferResized = false;

  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VkContext::SetWindowAndResizeCallback(GLFWwindow* inWindow) {
  window = inWindow;

  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}

void VkContext::RecreateSwapChain() {
  vkDeviceWaitIdle(device);

  // handle minimizing to tray
  int width = 0, height = 0;
  glfwGetFramebufferSize(window, &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(window, &width, &height);
    glfwWaitEvents();
  }

  pCreator->GetBuilderByType<VkSwapChainBuilder>()->Destroy();

  pCreator->GetBuilderByType<VkSwapChainBuilder>()->Build();
  pCreator->GetBuilderByType<VkImageViewsBuilder>()->Build();
  pCreator->GetBuilderByType<VkFramebuffersBuilder>()->Build();
}

void VkContext::RecordCommandBuffer(VkCommandBuffer commandBuffer,
    uint32_t imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;                   // Optional
  beginInfo.pInheritanceInfo = nullptr;  // Optional

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderPass;
  renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapChainExtent;

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapChainExtent.width);
  viewport.height = static_cast<float>(swapChainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapChainExtent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

  auto buffersSize = sizeof(testVertices[0]) * testVertices.size();
  auto vertexBufferMemoryWrapper = pBuffersManager->GetVertexBufferWrapper(buffersSize);

  VkBuffer vertexBuffers[] = { vertexBufferMemoryWrapper->Buffer };
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

  vkCmdDraw(commandBuffer, static_cast<uint32_t>(testVertices.size()), 1, 0, 0);
  // vkCmdDraw(commandBuffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void VkContext::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer,
    VkDeviceSize size) {

  // temp command buffer to execute our copy command
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0; // Optional
  copyRegion.dstOffset = 0; // Optional
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to copy buffer!");
  }

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(graphicsQueue);

  vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void VkContext::FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
  auto* appPtr = (VkContext*) glfwGetWindowUserPointer(window);
  appPtr->framebufferResized = true;
}