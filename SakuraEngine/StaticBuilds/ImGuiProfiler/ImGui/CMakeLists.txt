add_library(SImGui STATIC
imgui_impl_sdl.cpp
imgui_impl_vulkan.cpp
)

target_link_libraries(SImGui PUBLIC imgui::imgui SDL2::SDL2)