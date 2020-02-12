if(LINUX)
add_subdirectory(Linux)
else(WIN32)
add_subdirectory(Win32)
endif()

