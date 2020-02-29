message(STATUS "Generating SPA/UnitTestSPA.exe")

execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/SPA/anyToheader.py meta .json ${CMAKE_SOURCE_DIR}/SPAUnitTest 
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/SPA)
execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/SPA/ModuleCollector.py meta  ${CMAKE_SOURCE_DIR}/SPAUnitTest 
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/SPA)

project(UnitTestSPA VERSION 0.1.0)

add_subdirectory(plugin_0)
add_subdirectory(plugin_1)
add_subdirectory(plugin_main)

add_executable(UnitTestSPA UnitTestSPA.cpp)

target_link_libraries(UnitTestSPA SPA plugin_0 plugin_1 plugin_main)