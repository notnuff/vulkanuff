1. Preamble:  
   So, as Vulkan doesn't necessary need to interact with a window to
   present something on the screen (off-screen rendering, remember), it also
   doesn't have such functionality enabled by default - it is enabled with
   `Window System Integration` extensions, which are included at the
   instance creation step - if we asked glfw for extensions, it
   would return `VK_KHR_wayland_surface` and `VK_KHR_surface` in case of linux
   (wayland). The interaction with drawing surface, in our case, will be done
   through `VkSurfaceKHR` - it represents abstract and platform independent
   surface to present renders to.

2. Window surface creation:  
   Separate platform-specific extensions each provide a function for creating a
   VkSurfaceKHR object for the respective platform. That means, if we want
   to use surface to present something, we would need to create the same
   VkSurfaceKHR from each individual platform function - like for android,
   it would be `vkCreateAndroidSurfaceKHR`. Luckily, glfw can do it all for
   us with `glfwCreateWindowSurface`.

3. Checking for device presentation support:  
   Well, it's actually pretty simple - the implementation of Vulkan on the
   host can support WSI, but a GPU could not support it (again, some devices
   are designed for mining, so they don't need WSI). This support can be 
   determined by support of a queue family with `presentation capabilities`. 
   Yep, that's right, all of the support for such complex stuff depends on 
   just whether we can send a command to present something to a queue or not.
   Oh, and yes, it can be checked by `vkGetPhysicalDeviceSurfaceSupportKHR`

4. Creating the presentation queue:
   When we know the queue family which can support presentation, we just 
   need to create this queue, just like `graphicsQueue`, but it will be 
   `presentationQueue`. This queue, as graphicsQueue, will be created during 
   the logical device creation, so it also must be written to 
   `VkDeviceQueueCreateInfo`