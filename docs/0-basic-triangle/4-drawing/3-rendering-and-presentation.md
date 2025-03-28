At a high level, rendering a frame in Vulkan consists of a common set of steps:

- Wait for the previous frame to finish
- Acquire an image from the swap chain
- Record a command buffer which draws the scene onto that image
- Submit the recorded command buffer
- Present the swap chain image


    A core design philosophy in Vulkan is that synchronization of execution on the GPU is explicit. 
    The order of operations is up to us to define using various synchronization primitives which tell the driver the order we want things to run in. 
    This means that many Vulkan API calls which start executing work on the GPU are asynchronous, the functions will return before the operation has finished.

sync primitives:

- semaphores: blocks execution on the gpu, not blocks on host
- fence: blocks execution on host
- In summary, semaphores are used to specify the execution order of operations on the GPU while fences are used 
to keep the CPU and GPU in sync with each-other.

This is another milestone, so it's time to cleanup everything,
and possibly decompose current code to something meaningful