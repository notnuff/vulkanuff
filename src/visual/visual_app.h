#ifndef VISUAL_APP_H
#define VISUAL_APP_H

#include <GLFW/glfw3.h>

#include <memory>

#include "vulkan_ctx/initialization/vk_ctx_build_director.h"

// #include "vulkan_ctx/vk_context.h"
// #include "vulkan_ctx/initialization/builders/vk_ctx_builder_i.h"


class VisualApp {
public:
  void Run();

protected:
  void InitWindow();
  void InitVulkan();

  void DestroyVulkan();
  void DestroyWindow();

  void MainLoop();
  void Cleanup();

protected:
  GLFWwindow* window = nullptr;
  VkContext* vkCtx = nullptr;

protected:
  std::shared_ptr<VkCtxBuildDirector> vkCtxBuildDirector = nullptr;

};



#endif //VISUAL_APP_H
