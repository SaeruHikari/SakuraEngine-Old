message(STATUS "Generating SPA/UnitTestSPA.exe")

project(UnitTestSPA VERSION 0.1.0)

add_subdirectory(plugin_0)
add_subdirectory(plugin_1)
add_subdirectory(plugin_main)

add_executable(UnitTestSPA UnitTestSPA.cpp)

target_link_libraries(UnitTestSPA SPA plugin_0 plugin_1 plugin_main)