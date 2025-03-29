1. Preamble:  
   Well, it's a tough one topic, and very important at the same time (as all
   of them). First, lets look at definition from specs (I also copy-pasted
   them here and there, and I'm too lazy to quote every one of them):
    * The swap chain is essentially a queue of images that are waiting to be
      presented to the screen
    * A swapchain object (a.k.a. swapchain) **provides the ability to present
      rendering results to a surface**. Swapchain objects are represented by
      `VkSwapchainKHR`.
    * A swapchain is an abstraction for an array of presentable images that
      are associated with a surface. The presentable images are represented by
      `VkImage` objects created by the platform. One image is displayed at a
      time, but multiple images can be queued for presentation. An application
      renders to the image, and then queues the image for presentation to the
      surface.
    * How exactly the queue works and the conditions for presenting an image
      from the queue depend on how the swap chain is set up, but the **general
      purpose** of the swap chain is to **synchronize** the presentation of
      images
      with the refresh rate of the screen.

2. Checking for swap chain support:  
   So, now we understand that if we want to render to window surface, we
   need swap chain. And since presenting to screen is not part of Vulkan
   core, we need to acquire also `VK_KHR_swapchain` **device** extension.
   Yep, there are instance extensions, device extensions, instance validation
   layers, device properties, device features... And that's only beginning.
   Anyway, not all graphics cards are capable of presenting images directly to a
   screen, so we need to check it with `vkEnumerateDeviceExtensionProperties`.

3. Enabling device extensions:
   When we actually know that swap chain is supported by physical device, we
   need to acquire it while creating the logical device in
   `VkDeviceCreateInfo`, in enabledExtensionCount and ppEnabledExtensionNames to
   be clear.

4. Querying details of swap chain support:  
   If all this checking and creation process is not enough, there is more:  
   Just support of swap chain is not enough, we need also to check next
   properties to be sure that we actually can use swap chain:
    * Basic surface capabilities (min/max number of images in swap chain,
      min/max width and height of images) - Those can be acquired with
      `vkGetPhysicalDeviceSurfaceCapabilitiesKHR`
    * Surface formats (pixel format, color space) -
      `vkGetPhysicalDeviceSurfaceFormatsKHR`
    * Available presentation modes -`vkGetPhysicalDeviceSurfacePresentModesKHR`

5. Choosing settings of swap chain:
    * Surface format:
      Stored in a structure `VkSurfaceFormatKHR`.  
      There are a lot of possible formats, like if we wanted to support HDR,
      we might look at `VK_FORMAT_R16G16B16A16_UINT`, but we will stick with  
      `VK_FORMAT_B8G8R8A8_SRGB` and color space
      `VK_COLOR_SPACE_SRGB_NONLINEAR_KHR`.  
      When the images of swap chain will be created, they will be created
      with the format we specified.
    * Presentation mode:
      Stored in a structure `VkPresentModeKHR`.
      Represents the conditions for showing images to the screen.  
      There are such presentation modes:
        * `VK_PRESENT_MODE_IMMEDIATE_KHR` - images transferred to the screen
          right away, which may result in tearing.
        * `VK_PRESENT_MODE_FIFO_KHR` - swap chain is a queue where the
          display takes an image from the front of the queue when the display is
          refreshed and the program inserts rendered images at the back of the
          queue. Basically implements vertical sync.
        * `VK_PRESENT_MODE_FIFO_RELAXED_KHR` - instead of waiting for the next
          vertical blank, the image is transferred right away when it finally
          arrives. This may result in visible tearing.
        * `VK_PRESENT_MODE_MAILBOX_KHR` - instead of blocking the application
          when the queue is full, the images that are already queued are
          simply replaced with the newer ones. This mode can be used to render
          frames as fast as possible while still avoiding tearing, resulting in
          fewer latency issues than standard vertical sync. Basically implements
          triple buffering.

      Over all available modes, only `VK_PRESENT_MODE_FIFO_KHR` is
      guarantied to be present.
    * Swap extent:  
      Stored in a structure `VkExtent2D`.
      Here we just need to specify which resolution will be used - like
      beginning of our viewport and end (in pixels). Almost allways it will be
      just full window, but also we could make so called window-in-window.

6. Creating the swap chain:  
   Finally, after choosing everything we need, we are actually able to
   create swap chain. `VkSurfaceCapabilitiesKHR` that we acquired before
   have minImageCount and maxImageCount - we must acquire number of images
   in this range - and that will be our render targets in queue. Next, as usual,
   we just fill `VkSwapchainCreateInfoKHR` with our properties and create swap
   chain with `vkCreateSwapchainKHR`. Interesting property here -
   **imageUsage** - if we are rendering directly to the swap chain images, we
   will use `VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT`, but it is also possible
   to render to some other images first, to
   enable postprocessing for example - in that case **imageUsage** will be
   `VK_IMAGE_USAGE_TRANSFER_DST_BIT`, which means that some postprocessed
   image will be copied to our swap chain. There are other parameters
   to fill, but they are not as interesting, so if you want you can look at
   the code.

7. Retrieving the swap chain images:  
   With the swap chain images of this swap chain are created also
   implicitly. So, last step to use swap chain - is to get `VkImage`'s of the
   swap chain, to which we will render everything. We can get these handles
   with `vkGetSwapchainImagesKHR`.
      
   

