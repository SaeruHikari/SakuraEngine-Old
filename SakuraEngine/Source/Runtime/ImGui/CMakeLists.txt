Module(
    NAME ImGui
    TYPE Library
    SRC_PATH /Source #Default as Source
    DEPS
    DEPS_PUBLIC 
        RuntimeCore ECS 
        RenderGraph RenderGraphWebGPU RenderGraphVulkan
        ShaderCompiler
    INCLUDES_PUBLIC 
    INCLUDES 
        ${CMAKE_CURRENT_SOURCE_DIR}/Include/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
    LINKS
    LINKS_PUBLIC
)
