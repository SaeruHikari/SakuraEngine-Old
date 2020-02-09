message(STATUS "Generating UnitTestCore.exe")
message(STATUS "Generating UnitTestCoreUtils.exe")

add_executable(UnitTestCore UnitTestCore.cpp)
add_executable(UnitTestCoreUtils UnitTestCoreUtils.cpp)

target_link_libraries(UnitTestCore gtest gtest_main Core)
target_link_libraries(UnitTestCoreUtils gtest gtest_main EngineUtils Core)