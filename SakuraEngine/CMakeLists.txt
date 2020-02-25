cmake_minimum_required(VERSION 3.11)

project(Sakura VERSION 0.1.0)
execute_process(COMMAND python ../SPA/anyToheader.py meta .json ../SakuraEngine WORKING_DIRECTORY ../SPA)
execute_process(COMMAND python ../SPA/ModuleCollector.py meta  ../SakuraEngine WORKING_DIRECTORY ../SPA)

add_definitions(-D "SAKURA_CONSOLE_INOUT")
add_definitions(-D "SAKURA_DEBUG_EDITOR")
add_definitions(-D "SAKURA_DEBUG_GAME")
add_definitions(-D "SAKURA_GRAPHICS_PROFILING")

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

#GTest
enable_testing()

include_directories(
    ${PROJECT_SOURCE_DIR}
)
include_directories(
    ../Extern/include
)
include_directories(
    ../
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_subdirectory(Core)
add_subdirectory(Modules)
add_subdirectory(UnitTests)
add_subdirectory(Editor)
add_subdirectory(Framework)
add_subdirectory(Applications)

include(CPack)