add_subdirectory(Unix)
add_subdirectory(Win32)
add_subdirectory(Common)

add_library(EngineUtils EngineUtil_All.cpp)

target_link_libraries(EngineUtils PUBLIC xxhash SImGui) 