1. Preamble:  
   Index buffer - is effectively an array of pointers in the vertex buffer,
   which allows to reuse the same vertex for rendering several primitives,
   which are sharing this vertex. It allows to reorder vertex data, which
   will be passed to the graphics pipeline, and reuse vertex data of course -
   so that means we can save memory and time rendering complex objects.

2. Creation:
   Index buffer is created same way as the vertex buffer, but now it has  
   `VK_BUFFER_USAGE_INDEX_BUFFER_BIT` for usage in VkBufferCreationInfo.

3. Usage:
   Now that's just a little bit more complicated than creation - to use 
   index buffer in the pipeline, we need to bind it with 
   `vkCmdBindIndexBuffer`, where we specify the buffer itself and the type 
   of index data - either `VK_INDEX_TYPE_UINT16` or `VK_INDEX_TYPE_UINT32` - 
   it depends on how you save index data in your program. Also, to draw 
   something with index buffer, you need to use `vkCmdDrawIndexed` instead 
   of just `vkCmdDraw`.

4. TODO: use aliasing for better cache performance or reusing memory region