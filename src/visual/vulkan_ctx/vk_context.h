#ifndef VK_CONTEXT_H
#define VK_CONTEXT_H

// It's really important to include vulkan before GLFW,
// because otherwise we won't be able to create surface
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <memory>
#include <optional>
#include <vector>

#include "objects/vk_uniform_buffer_object.h"


class VkBuffersManager;
class VkCtxBuildDirector;

class VkContext {
public:
  void DrawFrame();
  void SetWindowAndResizeCallback(GLFWwindow* inWindow);

public:
  static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

protected:
  void RecreateSwapChain();
  void RecordCommandBuffer(VkCommandBuffer commandBuffer,
    uint32_t imageIndex);

protected:
  void PerformTransforms();
  void PerformVertexBufferCopying();
  void PerformIndexBufferCopying();

  void UpdateUniformBuffer();
  void PerformUniformBufferCopying();

  VkResult PerformSubmitDrawCommandsAndPresent(VkCommandBuffer commands, uint32_t imageIndex);

protected:
  void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

public:
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;

  GLFWwindow* window = nullptr;

  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkQueue graphicsQueue;

  VkSurfaceKHR surface;
  VkQueue presentQueue;


  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;

  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  std::vector<VkImageView> swapChainImageViews;

  VkRenderPass renderPass;

  VkDescriptorSetLayout descriptorSetLayout;
  VkPipelineLayout pipelineLayout;

  VkPipeline graphicsPipeline;

  std::vector<VkFramebuffer> swapChainFramebuffers;

  VkCommandPool commandPool;

  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;

  // need vector of buffers to be able to generate frames in flight
  std::vector<VkCommandBuffer> commandBuffers;

  // need vectors of semaphores and fences to be able to generate frames in flight
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;

  // will use current frame to check which frameOnFlight to use now
  uint32_t currentFrame = 0;
  bool framebufferResized = false;

  VkCtxBuildDirector* pCreator = nullptr;

public:
  std::shared_ptr<VkBuffersManager> pBuffersManager = nullptr;

protected:
  UniformBufferObject ubo;


protected:
  friend class VkCtxBuilderI;
};

#endif //VK_CONTEXT_H
