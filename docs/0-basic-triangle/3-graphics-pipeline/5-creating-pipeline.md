1. Creation of pipeline:  
   Now, after all the dirty job is done, we can just create the
   graphics pipeline. As always, using `VkGraphicsPipelineCreateInfo`, where
   we specify all the stuff from previous chapters:
    * Shader stages: the shader modules that define the functionality of the
      programmable stages of the graphics pipeline
    * Fixed-function state: all of the structures that define the
      fixed-function stages of the pipeline, like input assembly, rasterizer,
      viewport and color blending
    * Pipeline layout: the uniform and push values referenced by the shader
      that can be updated at draw time
    * Render pass: the attachments referenced by the pipeline stages and
      their usage

   From that we will get pipeline handle `VkPipeline`
