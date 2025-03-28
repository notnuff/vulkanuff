1. Introduction: 
```
   Vulkan separates the concept of physical and logical devices. A
   physical device usually represents a single complete implementation of
   Vulkan (excluding instance-level functionality) available to the host, of
   which there are a finite number. A logical device represents an instance of
   that implementation with its own state and resources independent of other
   logical devices.
```
   From this quote, we get to know that logical device is an instance of 
   physical device, so no global states like in OpenGL (which is cool). 
   Anyway, logical device is represented with `VkDevice`.

2. Queues for logical device:  
   So with creation of a logical device, we also need to specify how many 
   queues and from which families we would want to allocate, so we can 
   submit commands to that logical device. That is done with  
   `VkDeviceQueueCreateInfo`, where we need to appropriately 
   specify `queueFamilyIndex` and `queueCount` that we are going to use. 

3. Specifying used device features:  
   Here we specify, which physical device features we would want to use. Yep,
   that are those features from `vkGetPhysicalDeviceFeatures`. We specify 
   them with `VkPhysicalDeviceFeatures`.

4. Creating the logical device:  
   Nothing really mindblowing here - just creating the logical device with  
   `VkDeviceCreateInfo`, where we specify VkDeviceQueueCreateInfo and 
   VkPhysicalDeviceFeatures. Also, in this struct, we will specify 
   extensions of the physical device, that will be used by a logical device, 
   like VK_KHR_swapchain. // TODO: difference between extension and feature?

5. Getting handles to queues:  
   Logical device retrieves queues from physical device automatically, but 
   we still need to get handles from LD to access them. We can do it with  
   `vkGetDeviceQueue`, which will write handle of specific queue family 
   index to `VkQueue` (and don't forget that we can allocate several queues, 
   so we will need then to specify which one of them we want to use) 