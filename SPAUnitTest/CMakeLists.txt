message(STATUS "Generating SPA/UnitTestSPA.exe")

project(UnitTestSPA VERSION 0.1.0)

add_executable(UnitTestSPA UnitTestSPA.cpp)

target_link_libraries(UnitTestSPA SPA)