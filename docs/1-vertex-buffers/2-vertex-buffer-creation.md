1. Preamble:  
   Buffers are the memory regions that are used for storing data, that will
   be read by GPU. We need manually allocate memory for them.

2. Creation of Vertex Buffers:  
   Buffers are created with the help of `VkBufferCreateInfo` - in this
   structure, we specify **size** of the buffer in bytes, **usage** - like
   VK_BUFFER_USAGE_VERTEX_BUFFER_BIT or VK_BUFFER_USAGE_INDEX_BUFFER_BIT (or
   both of them, buffers can and will be used with several bits specified),
   and **sharing mode** - either buffer will be accessed from only one
   different _queue families_ on the same time. As result we get `VkBuffer` 
   handle.

3. Memory requirements for the Buffer:  
   Creating the buffer doesn't include memory allocation - and unlike with
   CPU, we there can be several types of memory to allocate from for
   concrete buffer - we can ask Vulkan to fill `VkMemoryRequirements` struct
   with `vkGetBufferMemoryRequirements` - this struct will contain
   required **size** of memory in bytes, **alignment** - offset in bytes where
   the buffer begins in the allocated region of memory, and
   **memoryTypeBits** - bits types that are suitable for asked buffer. When
   we know which memory is suitable for the buffer, we can seek for the
   memory GPU actually has. For that we need to fill
   `VkPhysicalDeviceMemoryProperties` struct with  
   `vkGetPhysicalDeviceMemoryProperties`. This struct have **memoryTypes** -
   array of `VkMemoryType` structs, where is specified _heap_ and
   _properties_ of selected memory. We would want to have, along with
   properties needed in VkMemoryRequirements of the buffer,
   `VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT` and
   `VK_MEMORY_PROPERTY_HOST_COHERENT_BIT` properties, so that we could
   see this memory from the host, and map this memory to copy something to it.

4. Memory allocation for the Buffer:  
   With the right memory type selected, we can actually allocate this memory.
   As usual, `VkMemoryAllocateInfo` needed for that. In this struct we
   specify **size** of allocating memory and **memoryTypeIndex** - the first 
   one is clear, and the second one we get from 3rd step - the best memory, 
   that fits our needs. When memory actually allocated in `VkDeviceMemory` 
   struct, we bind it to the buffer with `vkBindBufferMemory`. 

5. 