cmake_minimum_required(VERSION 3.10)

add_definitions("-std=c++2a")
set(CMAKE_DEBUG_POSTFIX d)

#conan config
if(MSVC) 
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
execute_process(COMMAND conan install . --profile=win32profile_MTd.txt --build missing
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Conan)
else(${CMAKE_BUILD_TYPE} MATCHES "Release")
execute_process(COMMAND conan install . --profile=win32profile_MT.txt --build missing 
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Conan)
endif()
else(MSVC)
execute_process(COMMAND conan install . --build missing 
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Conan)
endif(MSVC)

include(${CMAKE_SOURCE_DIR}/Conan/conanbuildinfo.cmake)
conan_basic_setup()

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts -fPIC -march=native -O3 -pthread -lpthread ${VAR_CONFIG_FLAGS}")
    message(STATUS "${VAR_CONFIG_FLAGS}")
endif(MSVC)

add_subdirectory(SakuraEngine)
add_subdirectory(SPA)
add_subdirectory(SPAUnitTest)
add_subdirectory(DependencyGraph)
add_subdirectory(Extern)
