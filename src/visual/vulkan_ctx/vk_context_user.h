#ifndef VK_CONTEXT_USER_H
#define VK_CONTEXT_USER_H


class VkContext;
class VkContextUser {
public:
  virtual ~VkContextUser() = default;

  VkContext* Context();
  void Context(VkContext* contextPtr);

  protected:
  VkContext* pCtx = nullptr;
};



#endif //VK_CONTEXT_USER_H
