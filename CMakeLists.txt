cmake_minimum_required(VERSION 3.10)

set(VCPKG_PKGS eigen3 boost gtest spdlog glfw3 SDL2 fmt)
if(WIN32)
execute_process(COMMAND vcpkg install --triplet x64-windows ${VCPKG_PKGS})
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake)
else()
#execute_process(COMMAND $ENV{VCPKG_ROOT}/vcpkg install --triplet x64-linux ${VCPKG_PKGS})
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
endif()
message(STATUS "TOOL CHAIN: ${CMAKE_TOOLCHAIN_FILE}")
project(Sakura)
enable_testing()

find_package(Boost REQUIRED)
if(Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS})
endif()
find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
include_directories(${Vulkan_INCLUDE_DIRS})
endif()
find_package(glfw3 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
else()
#execute_process(COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/Extern/ExternGet.py WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Extern)
endif()
set(CMAKE_DEBUG_POSTFIX d)


if(MSVC)     
    foreach(var 
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
      )
      if(${var} MATCHES "/MT")
        string(REGEX REPLACE "/MT" "/MD" ${var} "${${var}}")
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

#features
set(VAR_CONFIG_FLAGS " -D DEBUG")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)


if(MSVC)
    message(STATUS "MSVC Compiler cl.exe")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS") #Multi Thread Build Enable
else(MSVC)
    message(STATUS "Use GCC Compiler gcc.exe")
    add_definitions(-msse4.2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fconcepts -fPIC -march=native -O3 -pthread -lpthread ${VAR_CONFIG_FLAGS}")
    message(STATUS "${VAR_CONFIG_FLAGS}")
endif(MSVC)

add_subdirectory(Extern)
add_subdirectory(DependencyGraph)
add_subdirectory(SPA)
add_subdirectory(SPAUnitTest)
add_subdirectory(SakuraEngine)



