1. Preamble:  
   Here I will try to explain something about the two programmable and
   necessary pipeline stages to actually draw something - vertex and
   fragment shaders. First of all, Vulkan uses `SPIR-V` - it's a bytecode
   format for shaders, and the cool thing of it is that HLSL and GLSL can be
   compiled to such format, so we are truly independent of which high level
   shader language we will choose. The downfall - we will need to compile
   shaders to SPIR-V manually.
2. Vertex shader:  
   Actually, I will just copypaste explanation from the tutorial:
   ```
   The vertex shader processes each incoming vertex. It takes its attributes,
   like model space position, color, normal and texture coordinates as input.
   The output is the final position in clip coordinates and the attributes that
   need to be passed on to the fragment shader, like color and texture
   coordinates. These values will then be interpolated over the fragments by the
   rasterizer to produce a smooth gradient.

   Each vertex shader invocation operates on one vertex and its associated
   vertex attribute data, and outputs one vertex and associated data.
   ```
   So, this shader gives us position of a vertex in **clip coordinates** - 
   this are the coordinates which then are converted in normalized device 
   coordinates - those which have range of [-1; 1] and will be used to draw 
   vertices on the screen.  
   And, as you understood, that's the shader where all lineal algebra stuff 
   happening - matrices multiplication, transforming and so on. I won't 
   write a lot about this in here, as that's quite another topic.
   The results of this shader we will have to write to built-in 
   `gl_Position` variable, which is result position for vertex, and also in 
   our user-defined variables, which will be used in later shaders (fragment,
   at least).  

3. Fragment shader:  
   That's basically the shader invoked **per each pixel** of the output geometry,
   which are called **fragments**. The applicability of this shader, as well 
   as others, is described in [1-basics.md](1-basics.md). This shader can 
   directly use variables described in vertex shader. The conversion is 
   simple:
   `layout(location = 0) out vec3 fragColor;` in vertex shader will be 
   accessed like `layout(location = 0) in vec3 fragColor;` in fragment 
   shader. No buffers for this, fortunately 

4. Compiling the shaders:  
   Just using glslc as we would use gcc, nothing special here

5. Loading the shaders:  
   Now, when shaders are compiled, we want to include them in our graphics 
   pipeline and use. For that, in our pipeline creation 
   module/function/class/whatever, we need to firstly load the compiled 
   SPIR-V shader to some buffer, and then create `VkShaderModule` wrapper 
   over this loaded shader, so it can be used by Vulkan. 
   This wrapper is created with `VkShaderModuleCreateInfo`, where we just 
   specify the code from buffer.

6. Shaders stage creation in pipeline:
   When `VkShaderModule` wrappers are created, we need to actually include 
   them into pipeline. It is done via `VkPipelineShaderStageCreateInfo`. 
   Everything there is self-explanatory, but `pSpecializationInfo` worth 
   mentioning - in this struct, we can set constants for shader, that will 
   be passed to it after pipeline creation - and this way it's much easier and 
   more efficient than passing constants via buffers at the runtime. 

   