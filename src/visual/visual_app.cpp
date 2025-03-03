#include "visual_app.h"

#include "vulkan_ctx/vk_context.h"
#include "vulkan_ctx/initialization/vk_ctx_build_director.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

void VisualApp::Run() {
  InitWindow();
  InitVulkan();

  MainLoop();

  Cleanup();
}

void VisualApp::InitWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Vulkan Window", nullptr, nullptr);

}

void VisualApp::InitVulkan() {
  if (!vkCtxBuildDirector) vkCtxBuildDirector = std::make_shared<VkCtxBuildDirector>();

  vkCtxBuildDirector->SetWindow(window);
  vkCtxBuildDirector->InitBuilders();

  vkCtx = vkCtxBuildDirector->CreateContext();
}

void VisualApp::DestroyVulkan() {
  vkCtxBuildDirector->DestroyContext(vkCtx);
}

void VisualApp::DestroyWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void VisualApp::MainLoop() {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    vkCtx->DrawFrame();
  }
}

void VisualApp::Cleanup() {

}