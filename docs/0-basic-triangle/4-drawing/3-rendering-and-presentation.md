1. Preamble:  
   Quite a hard, but interesting topic. This will be composing of all
   previous steps to make something actually work.

2. Outline of a frame:
   In a nutshell, to render a frame we need to perform this steps per-frame:
    * Wait for previous frame to finish
    * Acquire an image from the swap chain
    * Record a command buffer which draws the scene onto that image
    * Submit the recorded command buffer
    * Image is rendered with our command buffer
    * Present rendered image to the swap chain image

3. Synchronization:
   Vulkan, just like directx12, needs to be synchronized for each step of
   presenting the image. That means, we need to take care of synchronizing
   steps like:

    * Waiting for the previous frame to finish
    * Acquiring an image from the swap chain
    * Executing commands that draw onto the acquired image
    * Presenting that image to the screen for presentation, returning it to the
      swapchain

   The thing is that when we submit some work to the GPU, our program don't
   stop and wait for the GPU to finish the work - it all performs
   **asynchronously** - and so, we need to synchronize it with our hands.
   That's just like parallel programming, the difference is that we have to
   sync not multiple cores, but rather the host and GPU.

4. Synchronization primitives that Vulkan provides us with:
    1. Semaphores:  
       Semaphores can be used to control resource access across multiple queues.
       Examples of queues are the graphics queue and the presentation queue.
       Semaphores are used both to order work inside the same queue and between
       different queues. The example of using semaphores - is that we need to
       use semaphores to force presentation queue start presenting an image from
       graphics queue **only after** this image is **rendered and ready to be
       presented**.  
       Now, about semaphores itself: the structure used here is
       `VkSemaphore` - and it can have 2 states - signaled or unsignaled. So,
       just like with usual mutex, we can submit some queue to either
       **signal** the semaphore or **wait** for the semaphore with
       `vkQueueSubmit` - and it will do exactly what we are expecting it to
       do - when queue is submitted with semaphore passed as _wait_ one, it
       won't start executing until the other queue with semaphore passed as
       _signal_ one signals the semaphore. After the waiting queue is
       finally being signaled to continue the work, it will set the
       semaphore to unsignaled state automatically.
       Created with `VkSemaphoreCreateInfo`

    2. Fences:  
       As you understood, semaphores control execution order only on the
       GPU. Now, Vulkan also has synchronization primitives to sync host and
       the GPU - `VkFence`. It works exactly like the semaphore, but it
       signals if some queue is done working on the GPU side. We submit the
       queue with the fence with the same `vkQueueSubmit`, but now queue will
       only signal when it's done working, and on the host side we will need
       to use `vkWaitForFence` to block execution. We need to reset unsignaled
       state for the fence manually with `vkResetFences`. Created with
       `VkFenceCreateInfo`

    3. Events:  
       Events provide a fine-grained synchronization primitive which can be
       signaled either within a command buffer or by the host, and can be waited
       upon within a command buffer or queried on the host. Events can be used
       to control resource access within a single queue.

    4. Pipeline Barriers:  
       Pipeline barriers also provide synchronization
       control within a command
       buffer, but at a single point, rather than with separate signal and wait
       operations. Pipeline barriers can be used to control resource access
       within a single queue.

    5. Render Pass Objects:  
       Render pass objects provide a
       synchronization framework for rendering
       tasks, built upon the concepts in this chapter. Many cases that would
       otherwise need an application to use other synchronization primitives can
       be expressed more efficiently as part of a render pass. Render pass
       objects can be used to control resource access within a single queue.

5. Waiting for the previous frame:  
   Basically, [here](https://www.reddit.com/r/vulkan/comments/nbu94q/what_exactly_is_the_definition_of_frames_in_flight/)
   is good explanation what we are doing in this step. In a nutshell - we
   have several frames in flight, that allow us to make buffering so that
   image on the screen isn't ripping apart. But, we still can't render more
   than one image at the time, although we can have several images to
   present before previous ones actually presented. So, lets say that CPU
   calculated all the matrices and game stuff, and submitted the work to the
   GPU. Now, CPU can start to calculate next frame, but since the GPU is
   busy right now, there is no need to actually do it - as all the
   calculations would be outdated before we can submit another work to the
   GPU. So, practically, we need to wait for the previous frame to finish
   rendering (not presenting!) to swap chain image before starting to
   calculate and submit next image. This is done with fence, and not that hard.

6. Acquiring next image from the swap chain:
   So, after GPU has done its work, it can be used to render next image in
   the swap chain. It is done via `vkAcquireNextImageKHR` - we can see that
   all stuff from extensions (which swap chain is) end with KHR. It's
   important to notice, that this function will also signal our sync
   primitive (`VkSemaphore`) when the presentation engine will finish using
   this image - so that means, we can use this image again to render to.

7. Recording the command buffer:  
   Of course, we also need to record commands again for the given image.
   Before that reset the buffer with `vkResetCommandBuffer` and use command
   buffer recording
   from [2-command-pools-and-buffers.md](2-command-pools-and-buffers.md)

8. Submitting the command buffer:  
   Now, probably one of the most important parts in this chapter - we
   finally can and need to submit our buffer to the graphicsQueue. All the
   config and sync is done with `VkSubmitInfo`. Here we specify which
   semaphores to wait on before execution begins and in which stage(s) of the
   pipeline to wait. We want to wait here before image is released by the
   presentation engine and can be used for rendering to. That means that
   theoretically the implementation can already **start executing** our
   vertex shader and such while the image is **not yet available**. Also, here
   specify which semaphores to signal when this commands buffer is done
   executing - that will be semaphore for signalling that image is rendered and
   ready to be presented.

9. Subpass dependencies:  
   Described in [4-render-pass.md](../3-graphics-pipeline/4-render-pass.md)

10. Presentation:  
    Finally, we can set the presentation to the screen after all the
    rendering is performed. The presentation is configured with
    `VkPresentInfoKHR` - here we will set the semaphore which we need to
    wait before presenting - and it's basically that semaphore which says that
    image is already rendered. Also here we are setting swapchain to which 
    render will be presented, and the actual image index in swapchain. 
    Finally, we send the present info to present queue via 
    `vkQueuePresentKHR`. And here we are - _finally drawing something to 
    the screen_.