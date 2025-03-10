#include "vk_ctx_build_director.h"

#include <algorithm>

#include "../vk_context.h"
#include "builders/command_buffers/vk_command_buffers_builder.h"
#include "builders/command_pool/vk_command_pool_builder.h"
#include "builders/debug_messenger/vk_debug_messenger_builder.h"
#include "builders/framebuffers/vk_framebuffers_builder.h"
#include "builders/graphics_pipeline/vk_graphics_pipeline_builder.h"
#include "builders/image_views/vk_image_views_builder.h"
#include "builders/instance/vk_debug_instance_builder.h"
#include "builders/logical_device_builder/vk_logical_device_builder.h"
#include "builders/physical_device_picker/vk_physical_device_builder.h"
#include "builders/render_pass/vk_render_pass_builder.h"
#include "builders/surface/vk_surface_builder.h"
#include "builders/swap_chain/vk_swap_chain_builder.h"
#include "builders/sync_objects/vk_sync_objects_builder.h"
#include "builders/descriptor_set_layout/vk_descriptor_set_layout_builder.h"
#include "../buffers/vk_buffers_manager.h"

VkCtxBuildDirector::VkCtxBuildDirector() {}

void VkCtxBuildDirector::InitBuilders() {
  CreateAndPushBuilder<VkDebugInstanceBuilder>();
  CreateAndPushBuilder<VkDebugMessengerBuilder>();
  CreateAndPushBuilder<VkSurfaceBuilder>();
  CreateAndPushBuilder<VkPhysicalDeviceBuilder>();
  CreateAndPushBuilder<VkLogicalDeviceBuilder>();
  CreateAndPushBuilder<VkSwapChainBuilder>();
  CreateAndPushBuilder<VkImageViewsBuilder>();
  CreateAndPushBuilder<VkRenderPassBuilder>();

  CreateAndPushBuilder<VkDescriptorSetLayoutBuilder>();
  CreateAndPushBuilder<VkGraphicsPipelineBuilder>();

  CreateAndPushBuilder<VkFramebuffersBuilder>();
  CreateAndPushBuilder<VkCommandPoolBuilder>();
  CreateAndPushBuilder<VkCommandBuffersBuilder>();
  CreateAndPushBuilder<VkSyncObjectsBuilder>();
}

VkContext* VkCtxBuildDirector::CreateContext() {
  if (!pTargetWindow) throw std::runtime_error("No target window specified");

  auto* context = new VkContext();
  context->SetWindowAndResizeCallback(pTargetWindow);
  context->pCreator = this;


  for(const auto& builder : buildersContainer) {
    builder->Context(context);
    builder->Build();
  }

  context->pBuffersManager = CreateBuffersManager(context);

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

std::shared_ptr<VkBuffersManager> VkCtxBuildDirector::CreateBuffersManager(VkContext* forCtx) {
  auto buffersManager = std::make_shared<VkBuffersManager>();
  buffersManager->Context(forCtx);
  buffersManager->Init();

  return buffersManager;

}


void VkCtxBuildDirector::SetWindow(GLFWwindow* window) {
  pTargetWindow = window;
}