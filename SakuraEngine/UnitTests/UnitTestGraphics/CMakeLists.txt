message(STATUS "Generating UnitTestGraphics.exe")

add_executable(VkTriangle VkTriangle.cpp)
add_executable(HE he.cpp)
add_executable(vkImHW vkImHW.cpp)

target_link_libraries(HE PRIVATE fmt::fmt)

if(WIN32)
else()
find_package(glfw3 REQUIRED)
endif()

target_link_libraries(vkImHW ${Vulkan_LIBRARY} SImGui)


target_link_libraries(VkTriangle ${Vulkan_LIBRARY} glfw)
if(WIN32)
elseif(APPLE)
else()
target_link_libraries(VkTriangle -lGL -lGLU -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglfw ${Vulkan_LIBRARY})
endif()



