1. Preamble:  
   So, in previous chapters were discussed lots of stuff that we need to
   draw something, now we will discuss how to actually draw any geometry we
   want.

2. Vertex shader:  
   First of all, our vertex shader needs to get somehow data we want to pass
   to it. For that, we need to use layouts in the shader:
    * `layout(location = 0) in vec2 inPosition;`
    * `layout(location = 1) in vec3 inColor;`

   These are the _vertex attributes_ - properties, which are specified
   per-vertex in the vertex buffer.

3. Vertex data:  
   Well, all the info we want to pass to the shader needs to be
   created/loaded/etc in our C++ code, and it should match exactly what we
   have in the shader. Nothing special.

4. Binding descriptions:  
   Now, actually interesting topic - we need to specify **how** Vulkan should
   pass the info we want to pass to the vertex shader, once this info
   uploaded to the GPU memory. First of all, we have
   `VkVertexInputBindingDescription` struct, which specifies, quote:
   _the number of bytes between data entries and whether to move to the next
   data entry after each vertex or after each instance._ In this struct, we
   specify **stride** - bytes from one entry to the next, and **inputRate**
   specify `VK_VERTEX_INPUT_RATE_VERTEX` or `VK_VERTEX_INPUT_RATE_INSTANCE`.
   That actually can be useful when we pass, for example, vertices with
   format different from the one vertex shader needs - like imagine that we
   have packed into vertex also info about physics, to save space - that
   would need to make offset in the stride.
   Also, there is **binding** - field that specifies to which `VkBuffer`
   this description will be bound.

5. Attribute descriptions:  
   This description is specified via `VkVertexInputAttributeDescription`.
   Great explanation, quote: _An attribute description struct describes how to
   extract a vertex attribute from a chunk of vertex data originating from a
   binding description_. If we have 5 vertex attributes, we need 5 attribute
   descriptions. In this struct we need to specify:
    * **binding** - same as with binding descriptions
    * **location** - that's the location for attribute in the shader
    * **format** - we need to set type of attribute in the shader (like vec3),
      then in the code (like glm::vec3), and now - for Vulkan to understand
      this sh*t (like VK_FORMAT_R32G32B32_SFLOAT).
    * **offset** - the format parameter implicitly defines the byte size of
      attribute data, and the offset parameter specifies the number of bytes
      since the start of the per-vertex data to read from. Like, if we had 
      two doubles as vertex attributes, we would need to specify the offset 
      of 8 bytes for the second attribute here.

6. Pipeline vertex input:  
   Now, for a graphic pipeline to actually use our attributes and bindings 
   descriptors, we need to feed the pipeline creation function with appropriate
   `VkPipelineVertexInputStateCreateInfo` structure. That's it, now Vulkan 
   knows _how to pass_ our vertex data from memory to the vertex shader, and 
   _how to do it correctly_