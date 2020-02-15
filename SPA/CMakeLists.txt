cmake_minimum_required(VERSION 3.11)

project(SPA VERSION 0.1.0)

# Avoid in source building
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(FATAL_ERROR "In-source building is forbiden ! 
        (Please create a build/ dir inside the source dir or everywhere else)")
endif()

#Set to release build by default
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(SPAUnitTest)
#Needed on Windows
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

#Compiler flags: Cpp20/Open Warning
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -fvisibility=hidden")
# Output extra warning infos
if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
endif()

#Include Extern
include_directories(../Extern/include)

#Add whereami dependency
file(
    GLOB_RECURSE
    whereami_files
    include/whereami/src/*.c
    include/whereami/src/*.h
)

#Add src files
file (
    GLOB_RECURSE
    sources
    src/*.cpp
    src/*.h
)
file( # Only for IDE
    GLOB_RECURSE
    includes
    include/*.h
)
include_directories(include)

add_library(
    SPA 
    SHARED
    ${sources}
    ${includes}
    ${whereami_files}
)

if(UNIX)
    target_link_libraries(SPA dl)
endif()
