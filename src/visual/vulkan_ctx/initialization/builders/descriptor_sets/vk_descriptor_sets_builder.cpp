#include "vk_descriptor_sets_builder.h"

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
    const auto& uniformBufferWrapper =  pCtx->pBuffersManager->GetUniformBufferWrapper(i);

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBufferWrapper->BufferWrapper->Buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = VK_WHOLE_SIZE;//sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = pCtx->descriptorSets[i];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;

    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;

    descriptorWrite.pBufferInfo = &bufferInfo;
    vkUpdateDescriptorSets(pCtx->device, 1, &descriptorWrite, 0, nullptr);
  }

}

void VkDescriptorSetsBuilder::DoDestroy() {
  // just like with command buffers,
  // we don't need to destroy manually descriptor sets,
  // as they will be implicitly destroyed with descriptor pool


}