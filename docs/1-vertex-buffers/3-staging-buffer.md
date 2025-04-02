1. Preamble:  
   Staging buffer - is intermediate buffer that is used to transfer data
   from host accessible memory to the GPU-only accessible memory. We would
   like to do it because GPU-only accessible memory is much faster than CPU
   visible memory.

2. Transfer queue:  
   To transfer data from staging buffer to GPU memory we need to perform
   queue command. And the queue family which support such operation must
   have `VK_QUEUE_TRANSFER_BIT` queueFlag.

3. Creating a staging buffer and dependent buffers:  
   First of all, to use staging buffer, we need to created this buffer with
   such properties: for `VkBuffer` - usage should be
   VK_BUFFER_USAGE_TRANSFER_SRC_BIT, for `VkDeviceMemory` - such
   `VkMemoryPropertyFlags`:
   **VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT**.

   So, naturally, to other buffers that are using staging buffer will be
   appended new usage in VkBuffer - **VK_BUFFER_USAGE_TRANSFER_DST_BIT** -
   as this buffers will be targets for staging buffer, and also their memory
   will have **VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT** for
   VkMemoryPropertyFlags.

4. Performing content copy from one buffer to another:  
   Now, in Vulkan, although it is (usually) not possible to access GPU
   memory directly, we can use vulkan commands to perform copy. Now, to
   perform this commands, we need one-time-use command
   buffer - [2-command-pools-and-buffers.md](../0-basic-triangle/4-drawing/2-command-pools-and-buffers.md)
   it's not so different from this. Immediately after creating the command
   buffer we will put there `vkCmdCopyBuffer`. And then the same as with
   pipeline commands.

5. TODO: use VulkanMemoryAllocator