message(STATUS "Generating UnitTestCore.exe")
add_executable(UnitTestCore UnitTestCore.cpp)

target_link_libraries(UnitTestCore gtest gtest_main Core)