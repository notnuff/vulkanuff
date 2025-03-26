#include "vk_context.h"

#include <cstring>
#include <stdexcept>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>


#include "common/vertex.h"
#include "initialization/vk_ctx_build_director.h"
#include "initialization/builders/command_buffers/vk_command_buffers_builder.h"
#include "initialization/builders/framebuffers/vk_framebuffers_builder.h"
#include "initialization/builders/image_views_swapchain/vk_image_views_builder.h"
#include "initialization/builders/shared/vk_frames_in_flight.h"
#include "initialization/builders/swap_chain/vk_swap_chain_builder.h"
#include "buffers/vk_vertex_buffer_factory.h"
#include "buffers/vk_buffers_manager.h"
#include "objects/vk_uniform_buffer_object.h"

#include <stb_image.h>

#include "initialization/builders/depth_buffer/vk_depth_buffer_resources_builder.h"

void VkContext::DrawFrame() {
  // PerformTransforms();

  // oh fuck, we actually shouldn't copy this buffers every frame
  // TODO: use dirty bit
  PerformVertexBufferCopying();
  PerformIndexBufferCopying();
  // ~

  UpdateUniformBuffer();
  PerformUniformBufferCopying();

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

  RecordCommandBuffer(commandBuffers[currentFrame], imageIndex);
  result = PerformSubmitDrawCommandsAndPresent(commandBuffers[currentFrame], imageIndex);

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

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};

  renderPassInfo.clearValueCount = clearValues.size();
  renderPassInfo.pClearValues = clearValues.data();

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

  auto indexBufferSize = sizeof(testIndices[0]) * testIndices.size();
  auto indexBufferMemoryWrapper = pBuffersManager->GetIndexBufferWrapper(indexBufferSize);
  vkCmdBindIndexBuffer(commandBuffer, indexBufferMemoryWrapper->Buffer, 0, VK_INDEX_TYPE_UINT32);

  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
  vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(testIndices.size()), 1, 0, 0, 0);
  // vkCmdDraw(commandBuffer, static_cast<uint32_t>(testVertices.size()), 1, 0, 0);
  // vkCmdDraw(commandBuffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void VkContext::PerformTransforms() {
  for(auto& test_vertex : testVertices) {
    auto deltaAngleRadians = glm::radians(1.0);
    deltaAngleRadians *= 0.05;

    const auto x = test_vertex.pos.x;
    const auto y = test_vertex.pos.y;
    const auto sin = glm::sin(deltaAngleRadians);
    const auto cos = glm::cos(deltaAngleRadians);

    test_vertex.pos.x = x * cos - y * sin;
    test_vertex.pos.y = x * sin + y * cos;
  }
}

void VkContext::PerformVertexBufferCopying() {
  void* data;
  auto buffersSize = sizeof(testVertices[0]) * testVertices.size();

  const auto& stagingBufferWrapper = pBuffersManager->GetStagingBufferWrapper(buffersSize);
  const auto& vertexBufferWrapper = pBuffersManager->GetVertexBufferWrapper(buffersSize);

  vkMapMemory(device, stagingBufferWrapper->Memory, 0, buffersSize, 0, &data);
  memcpy(data, testVertices.data(), (size_t) buffersSize);
  vkUnmapMemory(device, stagingBufferWrapper->Memory);

  CopyBuffer(stagingBufferWrapper->Buffer, vertexBufferWrapper->Buffer, buffersSize);

}

void VkContext::PerformIndexBufferCopying() {
  auto buffersSize = sizeof(testIndices[0]) * testIndices.size();

  const auto& stagingBufferWrapper = pBuffersManager->GetStagingBufferWrapper(buffersSize);
  const auto& indexBufferWrapper = pBuffersManager->GetIndexBufferWrapper(buffersSize);

  void* data;
  vkMapMemory(device, stagingBufferWrapper->Memory, 0, buffersSize, 0, &data);
  memcpy(data, testIndices.data(), (size_t) buffersSize);
  vkUnmapMemory(device, stagingBufferWrapper->Memory);

  CopyBuffer(stagingBufferWrapper->Buffer, indexBufferWrapper->Buffer, buffersSize);
}

void VkContext::UpdateUniformBuffer() {
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  auto deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

  ubo.model = glm::rotate(glm::mat4(1.0f), deltaTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);
  ubo.proj[1][1] *= -1;
}

void VkContext::PerformUniformBufferCopying() {
  memcpy(pBuffersManager->GetUniformBufferWrapper(currentFrame)->MappedMemory, &ubo, sizeof(ubo));
}

VkResult VkContext::PerformSubmitDrawCommandsAndPresent(VkCommandBuffer commands, uint32_t imageIndex) {
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commands;

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

  return vkQueuePresentKHR(presentQueue, &presentInfo);
}

void VkContext::PerformImageLayoutTransition(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
  VkCommandBuffer commandBuffer = BeginFastCommand();

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;

  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

  barrier.image = image;

  if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    const auto& depthBuff = pCreator->GetBuilderByType<VkDepthBufferResourcesBuilder>();
    if (depthBuff->IsFormatHasStencil(format)) {
      barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
  } else {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  }

  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStageMask;
  VkPipelineStageFlags destinationStageMask;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    sourceStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  } else {
    throw std::invalid_argument("unsupported layout transition!");
  }


  vkCmdPipelineBarrier(
    commandBuffer,
    sourceStageMask, destinationStageMask,
    0, // VK_DEPENDENCY_BY_REGION_BIT
    0, nullptr,
    0, nullptr,
    1, &barrier
);

  EndFastCommand(commandBuffer);
}


void VkContext::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer,
                           VkDeviceSize size) {

  const auto& commandBuffer = BeginFastCommand();

  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0; // Optional
  copyRegion.dstOffset = 0; // Optional
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

  EndFastCommand(commandBuffer);
}

void VkContext::CopyBufferToImage(VkBuffer buffer, VkImage image,
    uint32_t width, uint32_t height) {
  VkCommandBuffer commandBuffer = BeginFastCommand();
  VkBufferImageCopy region{};

  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;

  region.imageOffset = {0, 0, 0};
  region.imageExtent = { width, height, 1};

  vkCmdCopyBufferToImage(commandBuffer, buffer, image,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    1, &region);

  EndFastCommand(commandBuffer);
}

VkCommandBuffer VkContext::BeginFastCommand() {
  // temp command buffer to execute our command
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

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to start fast command!");
  }

  return commandBuffer;
}

void VkContext::EndFastCommand(const VkCommandBuffer& commandBuffer) {
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to end fast command!");
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