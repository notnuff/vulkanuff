1. Frames in flight:  
   Well, it's actually quite simple - we just need to create separate 
   command buffers, semaphores for image presentations, semaphores for image 
   rendering and fences, and that's it - we can render images in advance. I 
   guess, that's the best place to explain difference between frames in 
   flight and swap chain images:  
   So swap chain images support such thing as buffering, so there are no 
   tearing on the screen. Now, the GPU can render to only one of this images,
   while other is being displayed, and one is waiting to be displayed in 
   case of tripple buffering. The frames in flight, on the other hand, are 
   frames that we will send commands to render in advance to the GPU, so 
   just as previous frame of swap chain is rendered, GPU starts to render 
   next image without waiting for commands to be submitted from CPU.