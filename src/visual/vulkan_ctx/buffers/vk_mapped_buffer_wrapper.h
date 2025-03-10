#ifndef VKMAPPEDBUFFERWRAPPER_H
#define VKMAPPEDBUFFERWRAPPER_H
#include <memory>

#include "vk_buffer_wrapper.h"

class VkMappedBufferWrapper {
public:
  std::shared_ptr<VkBufferWrapper> BufferWrapper;
  void* MappedMemory;
};



#endif //VKMAPPEDBUFFERWRAPPER_H
