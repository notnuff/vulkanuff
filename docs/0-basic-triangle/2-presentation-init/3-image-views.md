1. Image views info:  
   Well, actually, that's quite simple one - to use any `VkImage`, we need
   to get a `VkImageView` of that image. It's kinda interface like
   std::string_view, but in case of Vulkan it's obligatory to use this. As
   allways, to get views, we need to specify `VkImageViewCreateInfo`. There
   are actually several interesting parameters can be configured:
    * **viewType** and **format** are just the same as in image
    * **components** field allows you to swizzle the color channels around. For
      example, you can map all of the channels to the red channel for a
      monochrome texture. You can also map constant values of 0 and 1 to a
      channel.
    * **subresourceRange** field describes what the image’s purpose is and which
      part of the image should be accessed. Here we can specify multiple layers
      which can be handy in stereographic 3D application, or mipmapping levels