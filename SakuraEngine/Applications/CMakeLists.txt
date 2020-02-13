if(${CMAKE_SYSTEM} MATCHES "Linux")
add_subdirectory(Linux)
else(WIN32)
add_subdirectory(Win32)
endif()

