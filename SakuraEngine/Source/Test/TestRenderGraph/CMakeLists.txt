Module(
    NAME TestRenderGraph
    TYPE Test
    SRC_PATH  /Source#Default as Source
    DEPS
    DEPS_PUBLIC 
        RuntimeCore ECS 
        RenderGraph RenderGraphWebGPU
        ShaderCompiler
        Tracker ImGui
    INCLUDES_PUBLIC
    LINKS
    LINKS_PUBLIC
)
