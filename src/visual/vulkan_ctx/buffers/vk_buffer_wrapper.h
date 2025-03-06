#ifndef VK_BUFFER_WRAPPER_H
#define VK_BUFFER_WRAPPER_H

#include <vulkan/vulkan.h>

class VkBufferWrapper {
public:
    VkBuffer Buffer;
    VkDeviceMemory Memory;
    VkDeviceSize Size;
};



#endif //VK_BUFFER_WRAPPER_H
