message(STATUS "Generating UnitTestGraphics.exe")
add_executable(UnitTestGraphics UnitTestGraphics.cpp)
add_executable(VkTriangle VkTriangle.cpp)
add_executable(SDLHello SDLHello.cpp)

find_package(glfw3 REQUIRED)

target_link_libraries(UnitTestGraphics gtest gtest_main gmock Core CGDModule)
target_link_libraries(VkTriangle ${CONAN_LIBS} -lGL -lGLU -lX11 -lXxf86vm -lXrandr -lpthread -lXi glfw ${Vulkan_LIBRARY} ApplicationModule)
target_link_libraries(SDLHello SDL2)

