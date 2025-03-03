#include "vk_vertex_buffer_builder.h"

#include <iostream>

#include "../../../common/vertex.h"

VkBufferCreateInfo& VkVertexBufferBuilder::GetVkBufferCreateInfo() {
  return bufferInfo;
}

void VkVertexBufferBuilder::DoBuild() {
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = sizeof(testVertices[0]) * testVertices.size();
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  bufferInfo.flags = 0;

  if (vkCreateBuffer(pCtx->device, &bufferInfo, nullptr, &pCtx->vertexBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to create vertex buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(pCtx->device, pCtx->vertexBuffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  if (vkAllocateMemory(pCtx->device, &allocInfo, nullptr, &pCtx->vertexBufferMemory) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate vertex buffer memory!");
  }

  vkBindBufferMemory(pCtx->device, pCtx->vertexBuffer, pCtx->vertexBufferMemory, 0);

}

void VkVertexBufferBuilder::DoDestroy() {
  vkDestroyBuffer(pCtx->device, pCtx->vertexBuffer, nullptr);
  vkFreeMemory(pCtx->device, pCtx->vertexBufferMemory, nullptr);
}

uint32_t VkVertexBufferBuilder::FindMemoryType(uint32_t typeFilter,
    VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(pCtx->physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
}