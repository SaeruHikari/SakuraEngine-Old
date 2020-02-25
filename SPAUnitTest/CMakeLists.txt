message(STATUS "Generating SPA/UnitTestSPA.exe")

execute_process(COMMAND python ../SPA/anyToheader.py meta .json ../SPAUnitTest WORKING_DIRECTORY ../SPA)
execute_process(COMMAND python ../SPA/ModuleCollector.py meta  ../SPAUnitTest WORKING_DIRECTORY ../SPA)

project(UnitTestSPA VERSION 0.1.0)

add_subdirectory(plugin_0)
add_subdirectory(plugin_1)
add_subdirectory(plugin_main)

add_executable(UnitTestSPA UnitTestSPA.cpp)

target_link_libraries(UnitTestSPA SPA plugin_0 plugin_1 plugin_main)