1. Preamble:  
   In order to complete our rendering program, we should consider resizing
   of the window. Now we can render something to swap chain, but the window
   can change the way that window surface is not compatible anymore with
   swap chain. So we need to configure that too.

2. Recreating the swap chain:
   So, in order to recreate swap chain when it becomes invalid, we need to:
    * wait for operations to finish with `vkDeviceWaitIdle`
    * recreate the swap chain
    * recreate swap chain image views
    * recreate framebuffers

   Also, it is possible to recreate swap chain without actually stopping all
   the rendering with vkDeviceWaitIdle - for that we need to use
   `oldSwapchain` in `VkSwapchainCreateInfoKHR` struct

3. Suboptimal or out-of-date swap chain:  
   Now, there are several cases when we need to recreate the swap chain -  
   `vkAcquireNextImageKHR` and `vkQueuePresentKHR` can return:
    * VK_ERROR_OUT_OF_DATE_KHR: The swap chain has become incompatible with
      the surface and can no longer be used for rendering. Usually happens after
      a window resize.
    * VK_SUBOPTIMAL_KHR: The swap chain can still be used to successfully
      present to the surface, but the surface properties are no longer matched
      exactly.  
   
   So, if we get one of those, we should recreate the swap chain

4. Handling resizes explicitly with GLFW callback:
   We also would like to set `glfwSetFramebufferSizeCallback`, as it's more 
   reliable trigger to update swap chain.
