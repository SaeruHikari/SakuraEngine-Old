Module(
    NAME Boids
    TYPE Test
    SRC_PATH  /Source #Default as Source
    DEPS
    DEPS_PUBLIC 
        RuntimeCore ECS 
        RenderGraph RenderGraphWebGPU RenderGraphVulkan
        ShaderCompiler
        Tracker
    INCLUDES_PUBLIC 
    LINKS
    LINKS_PUBLIC
)
