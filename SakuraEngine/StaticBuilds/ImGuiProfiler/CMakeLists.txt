add_subdirectory(ImGui)

add_library(ImGuiProfiler ImGuiVulkanSDL.cpp)

target_link_libraries(ImGuiProfiler PUBLIC SImGui)
