#include "vk_instance_builder.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <cstring>
#include <stdexcept>
#include <vector>

#include "vk_context.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector validationLayers = {
    "VK_LAYER_KHRONOS_validation",
};

VkInstanceBuilder::VkInstanceBuilder() {
  // initializing the app info
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Vulkan";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  // initializing the create info
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
}

void VkInstanceBuilder::DoBuild() {
  auto extensions = GetRequiredExtensions();

  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  createInfo.enabledLayerCount = 0;

  auto inst = pCtx->instance;
  if (vkCreateInstance(&createInfo, nullptr, &inst) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

void VkInstanceBuilder::DoDestroy() {
  vkDestroyInstance(pCtx->instance, nullptr);
}

std::vector<const char*> VkInstanceBuilder::GetRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char*> extensions(glfwExtensions,
                                      glfwExtensions + glfwExtensionCount);


  return extensions;
}
