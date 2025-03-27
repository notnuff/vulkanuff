#include "vk_surface_builder.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>

#include "vk_context.h"

void VkSurfaceBuilder::DoBuild() {
  if (glfwCreateWindowSurface(pCtx->instance, pCtx->window, nullptr, &pCtx->surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
}

void VkSurfaceBuilder::DoDestroy() {
  vkDestroySurfaceKHR(pCtx->instance, pCtx->surface, nullptr);
}