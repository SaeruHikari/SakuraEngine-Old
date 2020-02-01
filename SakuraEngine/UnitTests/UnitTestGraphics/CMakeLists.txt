message(STATUS "Generating UnitTestGraphics.exe")
add_executable(UnitTestGraphics UnitTestGraphics.cpp)

target_link_libraries(UnitTestGraphics gtest gtest_main)

