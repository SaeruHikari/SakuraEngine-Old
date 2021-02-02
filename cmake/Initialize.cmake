# Basic setup for toolchains
if(__COMPILER_PS5)
else()
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(cmake/Configurations.cmake)
include(cmake/TargetArchDetect.cmake)
include(CSharpUtilities)
include(CMakeParseArguments)

target_architecture(TARGET_ARCH)
if((TARGET_ARCH MATCHES "x86_64" OR TARGET_ARCH MATCHES "ia64") AND NOT OF_32BIT)
    set(ARCH_BIT 64)
    set(SAKURA_ARCH x64) #TODO: ARMS
else()
    set(ARCH_BIT 32)
    set(SAKURA_ARCH x86) #TODO: ARMS
endif()
message(STATUS "Target ARCH_BIT: ${ARCH_BIT}")

set(CLANG_DISABLED_WARNINGS 
    "-Wno-misleading-indentation -Wno-unused-variable -Wno-narrowing \
    -Wno-unused-function -Wno-macro-redefined -Wno-unused-value \
    -Wno-infinite-recursion -Wno-missing-braces \
    -Wno-macro-redefined -Wno-uninitialized -Wno-address-of-temporary \
    -Wno-c++11-narrowing -Wno-unknown-attributes -Wno-c++11-narrowing \
    -Wpointer-bool-conversion -Wimplicit-int-float-conversion \
    -Wno-tautological-pointer-compare -Wno-delete-abstract-non-virtual-dtor \
    -Wno-unused-label -Wno-zero-as-null-pointer-constant -Wno-extra-semi-stmt \
    -Wno-missing-prototypes -Wno-old-style-cast -Wno-reorder-ctor \
    -Wno-implicit-int-float-conversion -Wno-c++98-compat"
)

ADD_DEFINITIONS(-DMACRO)

set(API_IMPORT_DEF __attribute__\(\(visibility\(\"default\"\)\)\))
set(API_EXPORT_DEF __attribute__\(\(visibility\(\"default\"\)\)\))
set(API_HIDDEN_DEF __attribute__\(\(visibility\(\"hidden\"\)\)\))

## Target Platform
add_definitions(-D "SPDLOG_NO_EXCEPTIONS")
if(UNIX)
    add_definitions(-D "SAKURA_TARGET_PLATFORM_UNIX")
    if(APPLE)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_MACOS")
        set(SAKURA_PLATFORM "mac")
        set(macos 1)
        set(osx 1)
        set(mac 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES ANDROID)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_ANDROID")
        set(SAKURA_PLATFORM "android")
        set(android 1)
        SAKURA_REMOVE_DEF(SAKURA_HOST)
        SAKURA_REMOVE_DEF(SAKURA_USE_ISPC)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES Emscripten)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_EMSCRIPTEN")
        message(STATUS "Platform Web")
        set(SAKURA_PLATFORM "web")
        set(WA 1)   #Web
        set(web 1)
        SAKURA_REMOVE_DEF(SAKURA_HOST)
        SAKURA_REMOVE_DEF(SAKURA_USE_ISPC)
        SAKURA_REMOVE_DEF(SAKURA_USE_DXMATH)
    else(APPLE)
        add_definitions(-D "SAKURA_TARGET_PLATFORM_LINUX")
        set(SAKURA_PLATFORM "linux")
        set(linux 1)
    endif(APPLE)
elseif(WIN32)
    add_definitions(-D "SAKURA_TARGET_PLATFORM_WIN")
    set(SAKURA_PLATFORM "windows")
    message(STATUS "Platform WIN")
    set(windows 1)
elseif(__COMPILER_PS5)
    message(STATUS "Platform PS5")
    set(SAKURA_PLATFORM "prospero")
    add_definitions(-D "SAKURA_TARGET_PLATFORM_PLAYSTATION")
    link_libraries(ScePosix_stub_weak)
    SAKURA_REMOVE_DEF(SAKURA_HOST)
    SAKURA_REMOVE_DEF(SAKURA_USE_ISPC)
    set(API_IMPORT_DEF __declspec\(dllimport\))
    set(API_EXPORT_DEF __declspec\(dllexport\))
    set(API_HIDDEN_DEF )
endif(UNIX)


## Toolchain
if(MSVC)
    #add_definitions(-D "SAKURA_COMPILER_MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /permissive- /W3 /D _CRT_SECURE_NO_WARNINGS -DNOMINMAX") #Multi Thread Build Enable
    set(API_IMPORT_DEF __declspec\(dllimport\))
    set(API_EXPORT_DEF __declspec\(dllexport\))
    set(API_HIDDEN_DEF )
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(STATUS "MSVC Compiler clang-cl.exe")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CLANG_DISABLED_WARNINGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_DISABLED_WARNINGS}")
    else()
        message(STATUS "MSVC Compiler cl.exe")
    endif()
elseif(web)
    message(STATUS "Use EMCC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3 ${VAR_CONFIG_FLAGS} ${CLANG_DISABLED_WARNINGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CLANG_DISABLED_WARNINGS}")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(TARGET_SSE_AVX_NEON_LEVEL wasm-i32x4)
elseif(SAKURA_PLATFORM MATCHES "prospero")
    list(REMOVE_ITEM CMAKE_C_FLAGS "-std=gnu++17")
    list(REMOVE_ITEM CMAKE_CXX_FLAGS "-std=gnu++17")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -std=c++17 -O3 ${VAR_CONFIG_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CLANG_DISABLED_WARNINGS}")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #add_definitions(-D "SAKURA_COMPILER_CLANG")
    message(STATUS "Use CLANG Compiler Clang")
    add_definitions(-msse4.2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++ \
     -m64 -fPIC -march=native -O3 \
     -pthread ${VAR_CONFIG_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CLANG_DISABLED_WARNINGS}")
else()
    #add_definitions(-D "SAKURA_COMPILER_GCC")
    message(STATUS "Use GCC Compiler gcc")
    add_definitions(-msse4.2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -m64 -fPIC -march=native -O3 -pthread -lpthread ${VAR_CONFIG_FLAGS}")
    message(STATUS "${VAR_CONFIG_FLAGS}")
endif(MSVC)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Default Path Configuration

set(CMAKE_DEBUG_POSTFIX "_d")

#set(FULL_STATIC TRUE)

if(FULL_STATIC)
add_compile_definitions( 
    IMPORT_API=
    EXPORT_API= 
    HIDEEN_API=
)
elseif(windows OR prospero)
add_compile_definitions(
    IMPORT_API=__declspec\(dllimport\)
    EXPORT_API=__declspec\(dllexport\)
    HIDEEN_API=
)
elseif(osx)
add_compile_definitions( 
    IMPORT_API=__attribute__\(\(visibility\(\"default\"\)\)\) 
    EXPORT_API=__attribute__\(\(visibility\(\"default\"\)\)\) 
    HIDEEN_API=__attribute__\(\(visibility\(\"hidden\"\)\)\) 
)
endif()


set(vulkan FALSE)
if(windows)
find_package(Vulkan)
if(${Vulkan_FOUND})
    include_directories(${Vulkan_INCLUDE_DIRS})
    message(STATUS "Found VK SDK. Include Dir: ${Vulkan_INCLUDE_DIRS}")
    set(vulkan TRUE)
endif()
endif(windows)
