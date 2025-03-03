### swap surface formats:

        Each VkSurfaceFormatKHR entry contains a format and a colorSpace member. 
        The format member specifies the color channels and types. 
        For example, VK_FORMAT_B8G8R8A8_SRGB means that we store the B, G, R and alpha channels in that order with an 8 bit unsigned integer for a total of 32 bits per pixel. 
        The colorSpace member indicates if the sRGB color space is supported or not using the VK_COLOR_SPACE_SRGB_NONLINEAR_KHR flag. Note that this flag used to be called VK_COLORSPACE_SRGB_NONLINEAR_KHR in old versions of the specification.

### swap present modes for da program:

- **VK_PRESENT_MODE_IMMEDIATE_KHR:** Images submitted by your application are transferred to the screen right away, which may result in tearing.

- **VK_PRESENT_MODE_FIFO_KHR:** The only one guarantied swap chain mode. The swap chain is a queue where the display takes an image from the front of the queue when the display is refreshed and the program inserts rendered images at the back of the queue. If the queue is full then the program has to wait. This is most similar to vertical sync as found in modern games. The moment that the display is refreshed is known as "vertical blank".

- **VK_PRESENT_MODE_FIFO_RELAXED_KHR:** This mode only differs from the previous one if the application is late and the queue was empty at the last vertical blank. Instead of waiting for the next vertical blank, the image is transferred right away when it finally arrives. This may result in visible tearing.

- **VK_PRESENT_MODE_MAILBOX_KHR:** This is another variation of the second mode. Instead of blocking the application when the queue is full, the images that are already queued are simply replaced with the newer ones. This mode can be used to render frames as fast as possible while still avoiding tearing, resulting in fewer latency issues than standard vertical sync. This is commonly known as "triple buffering", although the existence of three buffers alone does not necessarily mean that the framerate is unlocked.

