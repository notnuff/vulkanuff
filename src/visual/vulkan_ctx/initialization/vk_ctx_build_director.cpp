#include "vk_ctx_build_director.h"

#include <algorithm>

#include "../vk_context.h"
#include "builders/command_buffers/vk_command_buffers_builder.h"
#include "builders/command_pool/vk_command_pool_builder.h"
#include "builders/debug_messenger/vk_debug_messenger_builder.h"
#include "builders/framebuffers/vk_framebuffers_builder.h"
#include "builders/graphics_pipeline/vk_graphics_pipeline_builder.h"
#include "builders/image_views_swapchain/vk_image_views_builder.h"
#include "builders/instance/vk_debug_instance_builder.h"
#include "builders/logical_device_builder/vk_logical_device_builder.h"
#include "builders/physical_device_picker/vk_physical_device_builder.h"
#include "builders/render_pass/vk_render_pass_builder.h"
#include "builders/surface/vk_surface_builder.h"
#include "builders/swap_chain/vk_swap_chain_builder.h"
#include "builders/sync_objects/vk_sync_objects_builder.h"
#include "builders/descriptor_set_layout/vk_descriptor_set_layout_builder.h"
#include "../buffers/vk_buffers_manager.h"
#include "builders/buffers_builder/vk_buffers_builder.h"
#include "builders/depth_buffer/vk_depth_buffer_resources_builder.h"
#include "builders/descriptor_pool/vk_descriptor_pool_builder.h"
#include "builders/descriptor_sets/vk_descriptor_sets_builder.h"
#include "builders/images/vk_texture_image_builder.h"
#include "builders/images/vk_texture_image_sampler_builder.h"
#include "builders/images/vk_texture_image_view_builder.h"

VkCtxBuildDirector::VkCtxBuildDirector() {}

void VkCtxBuildDirector::InitBuilders() {
  CreateAndPushBuilder<VkDebugInstanceBuilder>();
  CreateAndPushBuilder<VkDebugMessengerBuilder>();
  CreateAndPushBuilder<VkSurfaceBuilder>();
  CreateAndPushBuilder<VkPhysicalDeviceBuilder>();
  CreateAndPushBuilder<VkLogicalDeviceBuilder>();
  CreateAndPushBuilder<VkSwapChainBuilder>();
  CreateAndPushBuilder<VkImageViewsBuilder>();

  CreateAndPushBuilder<VkBuffersBuilder>();

  CreateAndPushBuilder<VkDescriptorSetLayoutBuilder>();

  CreateAndPushBuilder<VkCommandPoolBuilder>();

  CreateAndPushBuilder<VkDepthBufferResourcesBuilder>();

  CreateAndPushBuilder<VkRenderPassBuilder>();
  CreateAndPushBuilder<VkFramebuffersBuilder>();

  CreateAndPushBuilder<VkGraphicsPipelineBuilder>();


  CreateAndPushBuilder<VkTextureImageBuilder>();
  CreateAndPushBuilder<VkTextureImageViewBuilder>();
  CreateAndPushBuilder<VkTextureImageSamplerBuilder>();


  CreateAndPushBuilder<VkDescriptorPoolBuilder>();
  CreateAndPushBuilder<VkDescriptorSetsBuilder>();

  CreateAndPushBuilder<VkCommandBuffersBuilder>();
  CreateAndPushBuilder<VkSyncObjectsBuilder>();
}

VkContext* VkCtxBuildDirector::CreateContext() {
  if (!pTargetWindow) throw std::runtime_error("No target window specified");

  auto* context = new VkContext();
  context->SetWindowAndResizeCallback(pTargetWindow);
  context->pCreator = this;

  // first will set context for builders, as builder before can use something from builder after
  for(const auto& builder : buildersContainer) {
    builder->Context(context);
  }

  for(const auto& builder : buildersContainer) {
    builder->Build();
  }


  return context;
}

void VkCtxBuildDirector::DestroyContext(VkContext* ctx) {
  vkQueueWaitIdle(ctx->graphicsQueue);
  vkQueueWaitIdle(ctx->presentQueue);

  for(auto it = buildersContainer.rbegin(); it != buildersContainer.rend(); ++it) {
    (*it)->Context(ctx);
    (*it)->Destroy();
  }
}

void VkCtxBuildDirector::SetWindow(GLFWwindow* window) {
  pTargetWindow = window;
}