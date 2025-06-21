#include "vk_descriptor_sets_builder.h"

#include <array>

#include "initialization/builders/shared/vk_frames_in_flight.h"
#include "buffers/vk_buffers_manager.h"

void VkDescriptorSetsBuilder::DoBuild() {
  std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, pCtx->descriptorSetLayout);
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = pCtx->descriptorPool;
  allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
  allocInfo.pSetLayouts = layouts.data();

  pCtx->descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
  if (vkAllocateDescriptorSets(pCtx->device, &allocInfo, pCtx->descriptorSets.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate descriptor sets!");
  }


  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    const auto& uniformBufferWrapper = pCtx->pBuffersManager->GetUniformBufferWrapper(i);

    VkDescriptorBufferInfo uniformBufferInfo{};
    uniformBufferInfo.buffer = uniformBufferWrapper->BufferWrapper->Buffer;
    uniformBufferInfo.offset = 0;
    uniformBufferInfo.range = VK_WHOLE_SIZE;//sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptorWriteUniformBuffer{};
    descriptorWriteUniformBuffer.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWriteUniformBuffer.dstSet = pCtx->descriptorSets[i];
    descriptorWriteUniformBuffer.dstBinding = 0;
    descriptorWriteUniformBuffer.dstArrayElement = 0;

    descriptorWriteUniformBuffer.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWriteUniformBuffer.descriptorCount = 1;

    descriptorWriteUniformBuffer.pBufferInfo = &uniformBufferInfo;

    // ---------------------------------------------
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = pCtx->textureImageView;
    imageInfo.sampler = pCtx->textureSampler;

    VkWriteDescriptorSet descriptorWriteImage{};
    descriptorWriteImage.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWriteImage.dstSet = pCtx->descriptorSets[i];
    descriptorWriteImage.dstBinding = 1;
    descriptorWriteImage.dstArrayElement = 0;

    descriptorWriteImage.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWriteImage.descriptorCount = 1;

    descriptorWriteImage.pImageInfo = &imageInfo;

    // ---------------------------------------------
    const auto& lightingBufferWrapper = pCtx->pBuffersManager->GetLightingBufferWrapper(i);

    VkDescriptorBufferInfo lightingBufferInfo{};
    lightingBufferInfo.buffer = lightingBufferWrapper->BufferWrapper->Buffer;
    lightingBufferInfo.offset = 0;
    lightingBufferInfo.range = VK_WHOLE_SIZE;//sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptorWriteLightingBuffer{};
    descriptorWriteLightingBuffer.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWriteLightingBuffer.dstSet = pCtx->descriptorSets[i];
    descriptorWriteLightingBuffer.dstBinding = 2;
    descriptorWriteLightingBuffer.dstArrayElement = 0;

    descriptorWriteLightingBuffer.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWriteLightingBuffer.descriptorCount = 1;

    descriptorWriteLightingBuffer.pBufferInfo = &lightingBufferInfo;

    std::array descriptorWrites = { descriptorWriteUniformBuffer, descriptorWriteImage, descriptorWriteLightingBuffer };
    vkUpdateDescriptorSets(pCtx->device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
  }

}

void VkDescriptorSetsBuilder::DoDestroy() {
  // just like with command buffers,
  // we don't need to destroy manually descriptor sets,
  // as they will be implicitly destroyed with descriptor pool


}