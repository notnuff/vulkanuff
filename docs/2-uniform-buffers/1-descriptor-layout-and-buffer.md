1. Preamble:  
   With vertex buffers we can pass data per vertex, but we also often want
   to pass some global data to the shader. For this there are **resource
   descriptors** - basically it's like a handle or pointer into a resource,
   which can be accessed from the shader. So, we can have a buffer with some
   data in the GPU memory, which will be accessed from the vertex shader
   with a resource descriptor. To use a descriptor, we need:
    * specify a descriptor set layout during pipeline creation
    * allocate a descriptor set from a descriptor pool
    * bind the descriptor set during rendering

   In this case, _descriptor set layout_ is just like render pass specifies the
   types of attachments that will be accessed. And a _descriptor set_ specifies
   actual resources that will be bound to the descriptors, like a framebuffer
   specifies the actual image views to bind to render pass attachments. The
   descriptor set is then bound for the _drawing commands_ just like the vertex
   buffers and framebuffer.

   In here, only Uniform Buffer will be discussed, but as you can understand,
   descriptor sets can and will be used not only for that, but for many
   others global resources accesses.

2. Vertex shader:  
   So, to perform MVP transforms, we need to include the info about our
   UniformBufferObject into the shader. It can be used like that:
   ```glsl
      layout(binding = 0) uniform UniformBufferObject {
       mat4 model;
       mat4 view;
       mat4 proj;
   } ubo;
   ```
   Note the **binding** directive here - it will be used to bind actual
   buffers into pipeline.

3. Descriptor set layout:  
   Next, when we have UBO in Vertex shader, we need to also have same
   structure in cpp code:
   ```c++
   struct UniformBufferObject {
       glm::mat4 model;
       glm::mat4 view;
       glm::mat4 proj;
   };
   ```
   As we use 1 to 1 reflection of cpp and glsl code, later it will be
   possible to just copy our UBO from cpp code to appropriate `VkBuffer`
   memory.  
   The descriptor set layout is specified with `VkDescriptorSetLayoutBinding`,
   where we set:
    - **binding** that will be used in shader,
    - **descriptorType** like VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER or
      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
    - **descriptorCount** - in case if
      we want to pass an array of descriptors, for example for bones in
      skeletal animation.
    - **stageFlags** - bitfield to specify in which stage of pipeline this
      descriptor set will be used, like VK_SHADER_STAGE_VERTEX_BIT
    - **pImmutableSamplers** - samplers for images,
      see [2-image-view-and-sampler.md](../3-texture-mapping/2-image-view-and-sampler.md)

   Then, `VkDescriptorSetLayoutCreateInfo` is used to create the descriptor
   set layout, finally - in this struct we specify our
   `VkDescriptorSetLayoutBinding`, and this struct is used to create
   `VkDescriptorSetLayout` handle - this very handle will be used later in 
   pipeline layout creation. So, `VkPipelineLayoutCreateInfo` from [3-fixed-function-operations.md](../0-basic-triangle/3-graphics-pipeline/3-fixed-function-operations.md)
   will now also have `pSetLayouts` specified if we want to use the
   descriptor set layout in the pipeline - and in shader, of course.

4. Uniform buffer:  
   Well, here we will create uniform buffers for each frame-in-flight just 
   like in previous chapters, with a difference that uniform buffer won't be 
   copied via staging buffer, as UB will be rewritten every frame. Instead, 
   **persistent mapping** will be used on this buffer. I guess copying the 
   matrices to this buffer won't be that hard, so I won't describe it here. 
   Also, obviously, `VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT` is specified as 
   usage for the buffer.

5. Conclusions:
   In this chapter, we created the layout of descriptor set for UBO and 
   baked it into pipeline, as well as shader. Also, we created buffer for this 
   set and allocated memory for it. Now, we need to somehow combine the two 
   and start actually using the UBO