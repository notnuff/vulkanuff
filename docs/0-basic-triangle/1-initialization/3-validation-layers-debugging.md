1. What are validation layers:  
    So, as we know, Vulkan is veeery low-level API, with very little 
   overhead, and so there are no checks or debugging by default. But they 
   actually developed a great system - **validation layers**. This are 
   components that help debug and develop Vulkan apps, and as tutorial say, 
   most common use cases for them are:
   * Checking the values of parameters against the specification to detect misuse
   * Tracking creation and destruction of objects to find resource leaks
   * Checking thread safety by tracking the threads that calls originate from
   * Logging every call and its parameters to the standard output
   * Tracing Vulkan calls for profiling and replaying
   * (Actually, I should thy all of them, as so far I only used 1-2 of them)  

    And, most cool of all - is that all these layers can be disabled at any
   time if we don't want them - check vk_debug_instance_builder.cpp and 
   vk_instance_builder.cpp - with simple polymorphism we can either get 
   build for deployment or for debugging. 
2. Using validation layers:  
    Before we actually use any of these, we need to check if our requested 
   validation layers actually supported - and then specify them in `VkInstanceCreateInfo`
3. Message callback:  
   Also, we would want to control where do we print all the info we get from 
   validation layers (for example, to save them into file), and to control 
   which info we want to print - as by default VL will print EVERYTHING to 
   standard output - that means every single info about calls, successful 
   creations, etc, which is too much information. To resolve this issue, we 
   can use an explicit callback to this prints.  
   * For that, we need to add `VK_EXT_debug_utils` to required extensions in 
   instance creation.
   * Then we need to specify debugCallback and other stuff - you can see it 
     in appropriate vk_debug_instance_builder.cpp
   * There are several different message severities, from which we will use 
     only errors and warnings:
     * `VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT`: Diagnostic message
     * `VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT`: Informational message like 
     the creation of a resource
     * `VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT`: Message about behavior 
     that is not necessarily an error, but very likely a bug in your application
     * `VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT`: Message about behavior that 
     is invalid and may cause crashes
   
   * Also there are different message types:
     * `VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT`: Some event has happened that 
        is unrelated to the specification or performance
     * `VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT`: Something has happened that 
           violates the specification or indicates a possible mistake
     * `VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT`: Potential non-optimal use 
           of Vulkan

   * Now, all that stuff is specified in `VkDebugUtilsMessengerCreateInfoEXT`
   * and, finally, to create the message callback, we just need to manually 
     load the extension function and call it on our instance 
     with `VkDebugUtilsMessengerCreateInfoEXT`:
     ```auto func = (PFN_vkCreateDebugUtilsMessengerEXT) 
     vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"); 
     func(instance, debugMessenger, pAllocator); 
     ```
   That's actually quite hard, and it is recommended to read the 
   [docs](https://docs.vulkan.org/spec/latest/chapters/debugging.html#VK_EXT_debug_utils)


