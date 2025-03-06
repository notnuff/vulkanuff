#ifndef VK_CTX_BUILD_DIRECTOR_H
#define VK_CTX_BUILD_DIRECTOR_H

#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

class VkBuffersManager;
class VkCtxBuilderI;
class VkContext;

class VkCtxBuildDirector : std::enable_shared_from_this<VkCtxBuildDirector> {
public:
  VkCtxBuildDirector();

  ~VkCtxBuildDirector() = default;

public:
  void SetWindow(GLFWwindow* window);
  void InitBuilders();

public:
  VkContext* CreateContext();
  void DestroyContext(VkContext* ctx);

public:
  template<typename T>
  requires std::derived_from<T, VkCtxBuilderI>
  std::shared_ptr<T> GetBuilderByType();

protected:
  template<typename T>
  requires std::derived_from<T, VkCtxBuilderI>
  void CreateAndPushBuilder();

protected:
  std::shared_ptr<VkBuffersManager> CreateBuffersManager(VkContext* forCtx);

protected:
  std::vector<std::shared_ptr<VkCtxBuilderI>> buildersContainer;

protected:
  GLFWwindow* pTargetWindow = nullptr;

};

template <typename T>
requires std::derived_from<T, VkCtxBuilderI>
std::shared_ptr<T> VkCtxBuildDirector::GetBuilderByType() {
  for (const auto& builder : buildersContainer) {
    std::shared_ptr<T> builderCasted = std::dynamic_pointer_cast<T>(builder);
    if (builderCasted == nullptr) continue;

    return builderCasted;
  }

  return nullptr;
}

template <typename T>
requires std::derived_from<T, VkCtxBuilderI>
void VkCtxBuildDirector::CreateAndPushBuilder() {
  buildersContainer.push_back(std::make_shared<T>());
}

#endif  // VK_CTX_BUILD_DIRECTOR_H