#include "vk_descriptor_sets_builder.h"

#include <array>

#include "../shared/vk_frames_in_flight.h"
#include "../../../buffers/vk_buffers_manager.h"

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

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBufferWrapper->BufferWrapper->Buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = VK_WHOLE_SIZE;//sizeof(UniformBufferObject);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = pCtx->textureImageView;
    imageInfo.sampler = pCtx->textureSampler;

    VkWriteDescriptorSet descriptorWriteBuffer{};
    descriptorWriteBuffer.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWriteBuffer.dstSet = pCtx->descriptorSets[i];
    descriptorWriteBuffer.dstBinding = 0;
    descriptorWriteBuffer.dstArrayElement = 0;

    descriptorWriteBuffer.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWriteBuffer.descriptorCount = 1;

    descriptorWriteBuffer.pBufferInfo = &bufferInfo;

    VkWriteDescriptorSet descriptorWriteImage{};
    descriptorWriteImage.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWriteImage.dstSet = pCtx->descriptorSets[i];
    descriptorWriteImage.dstBinding = 1;
    descriptorWriteImage.dstArrayElement = 0;

    descriptorWriteImage.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWriteImage.descriptorCount = 1;

    descriptorWriteImage.pImageInfo = &imageInfo;

    std::array descriptorWrites = { descriptorWriteBuffer, descriptorWriteImage };
    vkUpdateDescriptorSets(pCtx->device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
  }

}

void VkDescriptorSetsBuilder::DoDestroy() {
  // just like with command buffers,
  // we don't need to destroy manually descriptor sets,
  // as they will be implicitly destroyed with descriptor pool


}