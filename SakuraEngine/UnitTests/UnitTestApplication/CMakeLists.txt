message(STATUS "Generating UnitTestApplication.exe")
add_executable(UnitTestApplication UnitTestApplication.cpp)

target_link_libraries(UnitTestApplication gtest gtest_main Core LinuxApp)