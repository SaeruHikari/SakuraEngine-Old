if(WIN32)

message(STATUS "MSVC: Win32 Application generating.")
add_library(Win32App Win32Engine.cpp Win32Window.cpp)
endif(WIN32)