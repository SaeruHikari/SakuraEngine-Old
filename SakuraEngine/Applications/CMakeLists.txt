add_library(ApplicationModule SHARED ApplicationModule.cpp)
if(${CMAKE_SYSTEM} MATCHES "Linux")
add_subdirectory(Linux)
target_link_libraries(ApplicationModule SPA SDL2::SDL2 SDL2::SDL2main LinuxApp)
else(WIN32)
add_subdirectory(Win32)
target_link_libraries(ApplicationModule SPA SDL2::SDL2 SDL2::SDL2main Win32App)
endif()

