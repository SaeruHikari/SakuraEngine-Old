cmake_minimum_required(VERSION 3.11)

project(DependencyGraph VERSION 0.1.0)

if (NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Debug")
add_definitions(-D "DAG_DEBUG")
#set(CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)


#features
set(VAR_CONFIG_FLAGS " -D DEBUG")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR} DAG")
#GTest
enable_testing()
add_subdirectory(DAGUnitTests)
endif()

include_directories(
    ../Extern/include
)