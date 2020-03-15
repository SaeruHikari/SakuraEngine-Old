cmake_minimum_required(VERSION 3.10)
#/home/saeruhikari/OpenLib/vcpkg/vcpkg install --triplet x64-linux eigen3 boost gtest spdlog glfw3 SDL2 fmt xxHash SDL2[vulkan]
set(VCPKG_PKGS eigen3 boost glfw3 SDL2 SDL2[vulkan] fmt vulkan-memory-allocator xxhash entt cpp-taskflow)

if(WIN32)
execute_process(COMMAND 
$ENV{VCPKG_ROOT}/vcpkg install --triplet x64-windows ${VCPKG_PKGS})
elseif(UNIX)
  if(APPLE)
    #execute_process(COMMAND $ENV{VCPKG_ROOT}/vcpkg install --triplet x64-osx ${VCPKG_PKGS})
  elseif(${CMAKE_SYSTEM_NAME}  MATCHES ANDROID)
  else(APPLE)
#execute_process(COMMAND $ENV{VCPKG_ROOT}/vcpkg install --triplet x64-linux ${VCPKG_PKGS})
  endif(APPLE)
endif(WIN32)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
message(STATUS "TOOL CHAIN: ${CMAKE_TOOLCHAIN_FILE}")

project(Sakura)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)

enable_testing()
find_package(Cpp-Taskflow CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED)
if(Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS})
endif()
find_package(Vulkan REQUIRED VERSION 1.2.131.2) 
if(Vulkan_FOUND)
include_directories(${Vulkan_INCLUDE_DIRS})
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

if (CMAKE_BUILD_TYPE STREQUAL Debug)
add_definitions(-D_DEBUG)
endif ()

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
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    message(STATUS "MSVC Compiler cl.exe")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS -DNOMINMAX") #Multi Thread Build Enable
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Use CLANG Compiler lang.exe")
    add_definitions(-msse4.2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++ -lc++fs -m64 -fPIC -march=native -O3 -pthread -lpthread ${VAR_CONFIG_FLAGS}")
else()
    message(STATUS "Use GCC Compiler gcc.exe")
    add_definitions(-msse4.2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -m64 -fPIC -march=native -O3 -pthread -lpthread ${VAR_CONFIG_FLAGS}")
    message(STATUS "${VAR_CONFIG_FLAGS}")
endif(MSVC)

include_directories(Extern/include/)

add_subdirectory(Extern)
add_subdirectory(DependencyGraph)
add_subdirectory(SPA)
add_subdirectory(SakuraTestProject)
add_subdirectory(SakuraEngine)



