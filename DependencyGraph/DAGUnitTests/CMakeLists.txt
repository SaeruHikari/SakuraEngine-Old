message(STATUS "Generating DependencyGraph/UnitTestDependencyGraph.exe")

include_directories(
    ../../Extern/include
)

add_executable(UnitTestDependencyGraph UnitTestDependencyGraph.cpp)