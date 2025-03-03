- vulkan has minimal driver overhead, so no checks
- validation layers - optional components to vulkan api

- How a Hook Works

For a function like vkCmdDraw:
    
    Application Call: The application calls vkCmdDraw.
    Dispatch Mechanism:
        The dispatch table points to the topmost layer’s implementation of vkCmdDraw.
        That layer processes the call (e.g., validating parameters).
        The layer forwards the call to the next layer (or directly to the driver if it is the last layer).
    Driver Call: Eventually, the call reaches the Vulkan driver, which executes the GPU command.


TOWATCH:

    There are a lot more settings for the behavior of validation layers than just the flags specified in the VkDebugUtilsMessengerCreateInfoEXT struct. 
    Browse to the Vulkan SDK and go to the Config directory. There you will find a vk_layer_settings.txt file that explains how to configure the layers.