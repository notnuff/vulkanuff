1. VkInstanceCreateInfo:  
    That's the beginning of any Vulkan application, so it must be created at 
   first. In fact, initialization of instance initializes Vulkan library 
   overall.  
    Instance is created with `VkInstanceCreateInfo` struct.  
    In this struct you must specify `VkApplicationInfo` -
    here will be stored different info about our app, like version, name, etc...
2. Enabling global extensions:
   Vulkan is a platform agnostic API, so we need an extensions to interact 
   with the window system on platform - for that we can use  
   `glfwGetRequiredInstanceExtensions`, which will give us required 
   extensions names for Vulkan
3. Creating of an instance is done with vkCreateInstance:  
   `VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);` - 
   that signature is similar to all Vulkan creation functions, and has next 
   parameters:
   * Pointer to struct with creation info
   * Pointer to custom allocator callbacks, if we are using such allocator
   * Pointer to the variable that stores the handle to the new object
4. Created instance should be deleted with `vkDestroyInstance(instance, 
nullptr);` - in my implementation, clearing will be done by the builder 
   itself, so DoDestroy function of the builder should be defined and overrided
