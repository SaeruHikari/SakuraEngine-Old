cmake_minimum_required(VERSION 3.11)

project(Sakura VERSION 0.1.0)
set(USE_CONAN true)

add_definitions(-D "SAKURA_DEBUG_EDITOR")
add_definitions(-D "SAKURA_DEBUG_GAME")

if (NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

#features
set(VAR_CONFIG_FLAGS " -D DEBUG")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

ADD_DEFINITIONS(-DMACRO)
if(UNIX)
    add_definitions(-D "SAKURA_TARGET_PLATFORM_UNIX")
    if(APPLE)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_OSX")
        set(SAKURA_TARGET_PLATFORM "OSX")
        set(OS_MACOS 1)
    elseif(${CMAKE_SYSTEM_NAME}  MATCHES ANDROID)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_ANDROID")
        set(SAKURA_TARGET_PLATFORM "Android")
        set(OS_ANDROID 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
        add_definitions(-D "SAKURA_TARGET_PLATFORM_FREEBSD")
        set(SAKURA_TARGET_PLATFORM "FreeBSD")
        set(OS_FREEBSD 1)
        set(BSD 1)
        set(OS_BSD 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES Emscripten)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_EMSCRIPTEN")
        set(SAKURA_TARGET_PLATFORM "Emscripten")
        set(WA 1)   #Web
        set(OS_WEBASSEMBLY 1)
    else(APPLE)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_LINUX")
        set(SAKURA_TARGET_PLATFORM "Linux")
        set(OS_LINUX 1)
    endif(APPLE)
elseif(WIN32)
    add_definitions(-D "SAKURA_TARGET_PLATFORM_WIN32")
    message(STATUS "Platform WIN32")
    set(SAKURA_TARGET_PLATFORM "Win32")
    set(OS_WINDOWS 1)
endif(UNIX)


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

if(MSVC)
    message(STATUS "MSVC Compiler cl.exe")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS") #Multi Thread Build Enable
else(MSVC)
    message(STATUS "Use GCC Compiler gcc.exe")
    add_definitions(-msse4.2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts -march=native -O3 -pthread -lpthread ${VAR_CONFIG_FLAGS}")
    message(STATUS "${VAR_CONFIG_FLAGS}")
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


#Eigen3 Include
if(USE_CONAN)
else()
    set( EIGEN3_INCLUDE_DIR "$ENV{EIGEN_INCLUDE}" )
    if( NOT EIGEN3_INCLUDE_DIR )
        message( STATUS "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
    else()
    include_directories("${EIGEN3_INCLUDE_DIR}")
    endif()
    message( STATUS "Add Eigen3 Root: ${EIGEN3_INCLUDE_DIR}")
endif()

#GTest
enable_testing()

include_directories(
    ${PROJECT_SOURCE_DIR}
)
include_directories(
    Extern/include
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_subdirectory(Core)
add_subdirectory(GraphicsInterface)
add_subdirectory(UnitTests)
add_subdirectory(Extern)
add_subdirectory(Framework)
add_subdirectory(Applications)

include(CPack)