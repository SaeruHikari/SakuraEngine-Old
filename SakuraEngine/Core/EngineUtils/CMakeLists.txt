add_subdirectory(Unix)
add_subdirectory(Win32)
add_subdirectory(Common)

file(GLOB_RECURSE COMMON_IMPL_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Common/*.cc 
    ${CMAKE_CURRENT_SOURCE_DIR}/Common/*.cpp
)

if(UNIX)
file(GLOB_RECURSE IMPL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Unix/*.cc)
message(STATUS "EngineUtils: Glob Unix Version implements!")
elseif(WIN32)
file(GLOB_RECURSE IMPL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Win32/*.cpp)
message(STATUS "EngineUtils: Glob Win32 Version implements!")
endif(UNIX)

add_library(EngineUtils ${IMPL_FILES} ${COMMON_IMPL_FILES})
