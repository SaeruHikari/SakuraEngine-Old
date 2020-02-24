if(WIN32)

message(STATUS "MSVC: Win32 Application generating.")
add_library(Win32App SHARED Win32Engine.cpp Win32Window.cpp)
target_link_libraries(Win32App Core)
endif(WIN32)