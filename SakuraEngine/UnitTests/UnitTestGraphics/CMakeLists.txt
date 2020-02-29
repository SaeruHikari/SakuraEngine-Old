message(STATUS "Generating UnitTestGraphics.exe")
add_executable(UnitTestGraphics UnitTestGraphics.cpp)
add_executable(VkTriangle VkTriangle.cpp)
add_executable(SDLHello SDLHello.cpp)

if(WIN32)
else()
find_package(glfw3 REQUIRED)
endif()

target_link_libraries(
UnitTestGraphics 
GTest::gtest GTest::gtest_main 
GTest::gmock GTest::gmock_main 
Core CGDModule)

if(WIN32)
target_link_libraries(VkTriangle ${Vulkan_LIBRARY} glfw)
else()
target_link_libraries(VkTriangle -lGL -lGLU -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglfw ${Vulkan_LIBRARY})
endif()
target_link_libraries(SDLHello 
SDL2::SDL2 SDL2::SDL2main 
CGD Core 
${Vulkan_LIBRARY})


