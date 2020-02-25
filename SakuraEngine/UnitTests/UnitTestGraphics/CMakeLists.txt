message(STATUS "Generating UnitTestGraphics.exe")
add_executable(UnitTestGraphics UnitTestGraphics.cpp)
add_executable(VkTriangle VkTriangle.cpp)
add_executable(SDLHello SDLHello.cpp)

target_link_libraries(UnitTestGraphics gtest gtest_main gmock Core CGDModule)
target_link_libraries(VkTriangle ${CONAN_LIBS} ${Vulkan_LIBRARY} ApplicationModule)
target_link_libraries(SDLHello SDL2)

