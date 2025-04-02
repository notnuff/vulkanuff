1. Preamble:  
   I'll just quote the tutorial: `The descriptor set layout from the previous
   chapter describes the type of descriptors that can be bound. In this chapter
   we’re going to create a descriptor set for each VkBuffer resource to bind it
   to the uniform buffer descriptor.`

2. Descriptor pool creation:  
   First of all, just like command buffers, descriptor sets must be
   allocated from pool, in this case - `VkDescriptorPool`. As usual,
   the pool is created with `VkDescriptorPoolCreateInfo`- in this struct, we
   need to describe which descriptor types our descriptor sets are going to
   contain and how many of them, using `VkDescriptorPoolSize` structures.
   The descriptorCount field here must be large enough to accommodate all
   descriptor sets that will be allocated from the
   pool. When using multiple frames in flight, each frame typically requires its
   own set of descriptor sets - which makes total sense, as the frame that
   16 ms later that another frame will have different matrices - and UBO
   accordingly.

3. Descriptor set creation: 
   Now, we can actually allocate `VkDescriptorSet` for each frame-in-flight. 
   As usual, `VkDescriptorSetAllocateInfo` is used - here we can specify to 
   allocate several sets at once. After VkDescriptorSet's allocated, we 
   still need to perform some setups. Specifically, we need to **bind** each 
   descriptor set to appropriate **buffer** from previous chapter.  
   `VkDescriptorBufferInfo` struct is used to specify buffer info for 
   descriptor. Here we set 
   - **buffer**, 
   - **offset**,
   - **range** - practically we want for descriptor to use only memory 
   of the buffer, which will contain our struct data - so we can specify 
   `sizeof(UBO)` for range. Or `VK_WHOLE_SIZE` if we want to use the whole 
   buffer.  
   
   Next, we have `VkWriteDescriptorSet` struct - this one will 
   actually be used to bind the buffer with the descriptor set. In here, we 
   specify:
   - **dstSet** - VkDescriptorSet handle for which we will perform binding 
     to the buffer
   - **dstBinding** - binding which is used for descriptor in the shader
   - **dstArrayElement** - this one is for descriptor arrays
   - **descriptorType** - same as we used in VkDescriptorSetLayoutBinding 
     during the creation of descriptor set layout, although we could 
     override it here.
   - **descriptorCount** - number of descriptors to update in array
   - **pBufferInfo** - our _VkDescriptorBufferInfo_ from earlier  
   
   And finally, it all comes to `vkUpdateDescriptorSets`. With that, 
   creation and configuration of descriptor sets is done.

4. Using descriptor sets:  
   Now, whe all the sets created, we need to bind it at command buffer for 
   drawing recording - it is done via `vkCmdBindDescriptorSets` before 
   drawing command - here is nice little [doc](https://docs.vulkan.org/spec/latest/chapters/descriptorsets.html#descriptorsets-binding)
   
5. Alignment requirements:
   This one quite obvious - all struct specified in C++ code should match 
   structs in shaders exactly

6. Multiple descriptor sets:
   Aside bindings in one set, there can be also multiple sets as well - in 
   glsl it will look like
  `layout(set = 0, binding = 0) uniform`. I will probably try it later with 
   this [guide](https://vkguide.dev/docs/chapter-4/descriptors_code/) 