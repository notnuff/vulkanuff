1. Preamble:  
   As I mentioned, every operation in vulkan requires command to be
   submitted and executed. And every command must be recorded in so-called
   **command buffer objects** - so, all of the commands we need for
   something will be executed with one single submit of CBO

2. Command pools:  
   **Command pools** are opaque objects that command buffer memory is allocated
   from, and which allow the implementation to amortize the cost of resource
   creation across multiple command buffers. Command pool is represented as  
   `VkCommandPool` handle, and created with `VkCommandPoolCreateInfo`. This
   struct takes **flags**:
    * VK_COMMAND_POOL_CREATE_TRANSIENT_BIT: Hint that command buffers are
      rerecorded with new commands very often (may change memory allocation
      behavior)
    * VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: Allow command
      buffers to be rerecorded individually, without this flag they all have to
      be reset together

   And **queueFamilyIndex** - command buffers are executed by submitting them on
   one
   of the _device queues_, like the graphics and presentation queues.
   Each command pool can only allocate command buffers that are submitted on a
   single type of queue, which makes total sense - we wouldn't be able to
   use a presentation commands on compute only queue.

4. Command buffer allocation:
   From command pools, we can allocate `VkCommandBuffer`. It's handled by
   pool, so we don't need to explicitly clear command buffer. Creation is
   done with `VkCommandBufferAllocateInfo`, which set commandPool to
   allocate from, commandBufferCount, and level - quite interesting field,
   which allows us to use:
    * VK_COMMAND_BUFFER_LEVEL_PRIMARY: Can be submitted to a queue for
      execution, but cannot be called from other command buffers.
    * VK_COMMAND_BUFFER_LEVEL_SECONDARY: Cannot be submitted directly, but can
      be called from primary command buffers.

5. Command buffer recording:  
   Now, we can start to record buffer, that will be submitted to a queue.
   It starts with `VkCommandBufferBeginInfo` that is used by
   `vkBeginCommandBuffer`, and ends with `vkCmdEndRenderPass`;

6. Recording rendering commands:
   To start drawing something, we need to record next commands:
    1. `vkCmdBeginRenderPass`, which takes `VkRenderPassBeginInfo` - here we
       specify which render pass we want to use, which framebuffer (yep, in
       render pass creation step we specified formats of framebuffers, and
       here we pass the framebuffers with this formats), renderArea - can be
       useful for partial rendering, and as we used
       VK_ATTACHMENT_LOAD_OP_CLEAR - we also should specify clear color for
       color attachment. In vkCmdBeginRenderPass itself we also specify if we
       will use secondary command buffers
    2. `vkCmdBindPipeline` - just specifying which pipeline we want to use
    3. `vkCmdSetViewport` and `vkCmdSetScissor` - if we used dynamic states,
       then we will have to specify also our `VkViewport` and `VkRect2D`
    4. `vkCmdDraw` - finally, we can use draw command. Here we specify:

       * vertexCount - well, it's obvious
       * instanceCount - used for instanced rendering
       * firstVertex - used as an offset into the vertex buffer, defines the
         lowest value of gl_VertexIndex.
       * firstInstance - used as an offset for instanced rendering, defines the
         lowest value of gl_InstanceIndex.
   
