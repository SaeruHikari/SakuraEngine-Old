cmake_minimum_required(VERSION 3.11)

add_definitions(-D "SAKURA_CONSOLE_INOUT")
add_definitions(-D "SAKURA_DEBUG_EDITOR")
add_definitions(-D "SAKURA_DEBUG_GAME")
add_definitions(-D "SAKURA_GRAPHICS_PROFILING")

if(MSVC)     
    foreach(var 
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
      )
      if(${var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
      endif()
    endforeach()    
endif(MSVC)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(TargetArchDetect)
target_architecture(TARGET_ARCH)
if((TARGET_ARCH MATCHES "x86_64" OR TARGET_ARCH MATCHES "ia64") AND NOT OF_32BIT)
        message(STATUS "x86_64 Target Architechture detected.")
        set(ARCH_BIT 64)
else()
        message(STATUS "x86 Target Architechture detected.")
        set(ARCH_BIT 32)
endif()


add_subdirectory(SakuraEngine)
add_subdirectory(ModuleDriver)
add_subdirectory(DependencyGraph)
add_subdirectory(Extern)