Module(
    NAME IMGUI_CORE
    TYPE StaticLibrary
    SRC_PATH 
        /Core/Source #Default as Source
    DEPS
    DEPS_PUBLIC 
        
    INCLUDES_PUBLIC 
        /Core/Include
    INCLUDES 
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Include/imgui
    LINKS
    LINKS_PUBLIC
)


Module(
    NAME ImGui
    TYPE Library
    SRC_PATH 
        /Source #Default as Source
    DEPS
    DEPS_PUBLIC 
        RuntimeCore 
        RenderGraph RenderGraphWebGPU RenderGraphVulkan
        ShaderCompiler
    INCLUDES_PUBLIC 
    INCLUDES 
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
    LINKS
    LINKS_PUBLIC IMGUI_CORE
)
