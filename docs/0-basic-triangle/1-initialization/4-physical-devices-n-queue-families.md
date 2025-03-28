1. Selecting the physical device:
   So, when Vulkan is initialized and configured, we want to pick a GPU,
   that supports all the features we need. In fact, we can even use several
   physical devices simultaneously to render something with Vulkan. A physical
   device usually represents a single complete implementation of Vulkan. To
   represent physical device, `VkPhysicalDevice` is used.
2. Check suitability of a device:  
   Every GPU have different properties, and so not every GPU will suit us -
   for example, we could not use a GPU that cannot render to the screen. So,
   to check that, we have different mechanisms to get info about GPU:
    * `VkPhysicalDeviceProperties`, which is retrieved with
      `vkGetPhysicalDeviceProperties` - this provides us with basic device
      properties like the name, type and supported Vulkan version.
    * `VkPhysicalDeviceFeatures`, which is retrieved with  
      `vkGetPhysicalDeviceFeatures` - this can give us optional features like
      support for geometryShader (again, GPU's for AI training don't
      need this at all, so it's optional), texture compression, 64 bit floats
      and multi viewport rendering.

3. Queue families:  
   In Vulkan, every and any operation (drawing, uploading textures,
   transferring vertices to the buffer) requires some commands to be
   submitted to queue, and not every queue can perform every command. There are
   different types of queues that originate from different **queue families** and
   each family of queues allows only a subset of commands. For example,
   there can be such queue families configuration:
    * One kind can do graphics, compute, transfer, and sparse binding
      operations, and you can create up to 16 queues of that type.
    * Another kind can only do transfer operations, and you can only create one
      queue of this kind. Usually, this is for asynchronously DMAing data
      between host and device memory on discrete GPUs, so transfers can be done
      concurrently with independent graphics/compute operations.
    * Last kind can create up to 8 queues that are only capable of compute
      operations.

   You can actually check this info for your own GPU, I just copypasted some 
   answer from stackoverflow for example. We can get queue family properties 
   with `vkGetPhysicalDeviceQueueFamilyProperties` into 
   `std::vector<VkQueueFamilyProperties>`. In such VkQueueFamilyProperties 
   structure we would want to seek for queue flags to see, which 
   capabilities this queue have.
   ```
    VK_QUEUE_GRAPHICS_BIT
    VK_QUEUE_COMPUTE_BIT
    VK_QUEUE_TRANSFER_BIT
    VK_QUEUE_SPARSE_BINDING_BIT
    VK_QUEUE_PROTECTED_BIT
    VK_QUEUE_VIDEO_DECODE_BIT_KHR
    VK_QUEUE_VIDEO_ENCODE_BIT_KHR
    VK_QUEUE_OPTICAL_FLOW_BIT_NV
   ```
   Here are some queues operations supported by queues. Ah, and don't forget, 
   that queue family is represented just by index. So, when we have indexes 
   of queue families, which are suitable for our needs, we can use those 
   indexes to create actual queues (this is similar to descriptor pools)