1. In short, to draw the first triangle we need to:

- Create a VkInstance
- Select a supported graphics card (VkPhysicalDevice)
- Create a VkDevice and VkQueue for drawing and presentation
- Create a window, window surface and swap chain
- Wrap the swap chain images into VkImageView
- Create a render pass that specifies the render targets and usage
- Create framebuffers for the render pass
- Set up the graphics pipeline
- Allocate and record a command buffer with the draw commands for every possible
  swap chain image
- Draw frames by acquiring images, submitting the right draw command buffer and
  returning the images back to the swap chain

2. Also, before we continue on with vulkan, we want to have window initialized
   (but not actually need - vulkan can draw everything off-screen).
   For this task we will stick with GLFW library, that creates all
   OS-independent
3. Also, I should notice, that I've used Builder pattern for creation of all
   required components, described in each section of this documentation. So,
   each component will be placed in appropriate builder - like instance will 
   be located at `src/visual/vulkan_ctx/initialization/builders/instance`
4. This docs won't describe in details what functions should be called and 
   in which order, it will just describe what we need for every step and 
   some usefull info.