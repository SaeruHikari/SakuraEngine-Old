add_library(Win32Editor Win32Editor.cpp)

add_subdirectory(Execute)

target_link_libraries(Win32Editor CoreModule)