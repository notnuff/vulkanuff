#include "vk_debug_instance_builder.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <cstring>
#include <stdexcept>
#include <vector>

#include "../../../vk_context.h"

const std::vector validationLayers = {
    "VK_LAYER_KHRONOS_validation",
};

void VkDebugInstanceBuilder::DoBuild() {
  CheckValidationLayersSupport();

  auto extensions = GetRequiredExtensions();
  createInfo.enabledExtensionCount = extensions.size();
  createInfo.ppEnabledExtensionNames = extensions.data();

  createInfo.enabledLayerCount = validationLayers.size();
  createInfo.ppEnabledLayerNames = validationLayers.data();

  if (vkCreateInstance(&createInfo, nullptr, &pCtx->instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

void VkDebugInstanceBuilder::CheckValidationLayersSupport() {
  for (auto* validationLayer : validationLayers) {
    if (IsValidationLayerAvailable(validationLayer)) continue;

    std::string message = "Extension not found: ";
    message += validationLayer;
    throw std::runtime_error(message);
  }
}

bool VkDebugInstanceBuilder::IsValidationLayerAvailable(const char* layerName) {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (auto& layer : availableLayers) {
    if (strcmp(layer.layerName, layerName) == 0) return true;
  }

  return false;
}

std::vector<const char*> VkDebugInstanceBuilder::GetRequiredExtensions() {
  auto extensions = VkInstanceBuilder::GetRequiredExtensions();
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  return extensions;
}
