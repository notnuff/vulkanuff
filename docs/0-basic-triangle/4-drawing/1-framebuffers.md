1. Preamble:  
   The attachments specified during render pass creation are bound by wrapping
   them into a `VkFramebuffer` object. A framebuffer object references all of the
   `VkImageView` objects that represent the attachments.

2. Creating framebuffers:
   The image that we have to use for the attachment depends on which image the
   swap chain returns when we retrieve one for presentation. That means that we
   have to create a framebuffer for all of the images in the swap chain and use
   the one that corresponds to the retrieved image at drawing time.
   Framebuffers are created with `VkFramebufferCreateInfo`, where we specify 
   target render pass, attachments of the framebuffer, width and height. 
   Framebuffers should be created after the render pass.