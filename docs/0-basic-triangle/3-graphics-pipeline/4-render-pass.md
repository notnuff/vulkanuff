1. Preamble:
   So, to finally create pipeline, we need to specify which framebuffer
   attachments will be used during rendering - that means, how many color,
   depth attachments we will be using, which formats they will have, how many
   samples to use for each of them and how their contents should be handled
   throughout the rendering operations. That all is done via `VkRenderPass`.
   A **render pass** is a sequence of rendering operations that define how image
   data is processed and written to a framebuffer. It groups together multiple
   rendering steps (such as clearing, drawing, and storing operations) to
   optimize GPU performance. A render pass usually consists of:
    * Attachments (color, depth, and stencil buffers)
    * Subpasses (smaller rendering stages within the pass)
    * Dependencies (ordering constraints between subpasses)

2. Attachment description:  
   To specify info about attachments, `VkAttachmentDescription` is used -
   here we can specify:
    * _format_ - RGBA as swap chain have for color, or
      VK_FORMAT_D32_SFLOAT for depth buffer
    * _samples_ - for multisampling
    * _loadOp_ and _storeOp_ - to determine what to do with the data in the
      attachment before rendering and after rendering - we can store, clear
      or set that we don't care about the data in attachment
    * _stencilLoadOp_ and _stencilStoreOp_ - same for stencil
    * _initialLayout_ and _finalLayout_ - that's quite interesting one:  
      As we know, for representing to surface `VkImage` is used, and
      although this objects have certain pixel format, layouts of pixels in
      memory can differ based on usage. Some of such layouts:
        * `VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL`: Images used as color
          attachment
        * `VK_IMAGE_LAYOUT_PRESENT_SRC_KHR`: Images to be presented in the swap
          chain
        * `VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL`: Images to be used as
          destination
          for a memory copy operation
        * Also, there is `VK_IMAGE_LAYOUT_GENERAL`, which, if I understand
          correctly, suitable for every usage

      It's important to notice, that we need to explicitly transition, aka
      change the layouts of images to specific ones if we want to use them -
      like in depth chapter we will transition image from undefined to optimal
      for depth buffering layout. And, of course, as you can understand,
      we specify right the same in initialLayout and finalLayout - we want
      to go from VK_IMAGE_LAYOUT_UNDEFINED to
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR layout after rendering, so that we can
      use this image to present on the screen

3. Subpasses and attachment references:
   ```
   A single render pass can consist of multiple subpasses. Subpasses are subsequent
   rendering operations that depend on the contents of framebuffers in previous
   passes, for example a sequence of post-processing effects that are applied one
   after another. If you group these rendering operations into one render pass,
   then Vulkan is able to reorder the operations and conserve memory bandwidth for
   possibly better performance.   
   ```

   Subpasses reference the attachments which we created with attachment
   descriptions via `VkAttachmentReference` - and so, that's how we can use
   color attachment or depth attachment in our render pass. The references
   then are being referenced in `VkSubpassDescription` struct, which also
   specifies in which pipeline this subpass can be used - like
   VK_PIPELINE_BIND_POINT_GRAPHICS or VK_PIPELINE_BIND_POINT_COMPUTE.

   The index of the attachment in this array is directly referenced from the
   fragment shader with the layout(location = 0) out vec4 outColor directive,
   so we can say that's the joining point where shader actually accesses our
   target
   render image.

   The following types of attachments can be referenced by a subpass:
    * pColorAttachments: Attachments that shaders put result to
    * pInputAttachments: Attachments that are read from a shader
    * pResolveAttachments: Attachments used for multisampling color attachments
    * pDepthStencilAttachment: Attachment for depth and stencil data
    * pPreserveAttachments: Attachments that are not used by this subpass, but
      for
      which the data must be preserved

4. Dependencies: //TODO

5. Render pass:  
   When everything is prepared, it's time to create `VkRenderPass`. It's 
   relatively simple - in `VkRenderPassCreateInfo`, just set the 
   `VkAttachmentReference`'s and `VkSubpassDescription`'s, and you are ready 
   to go further.