add_library(ApplicationModule SHARED ApplicationModule.cpp)
if(${CMAKE_SYSTEM} MATCHES "Linux")
add_subdirectory(Linux)
target_link_libraries(ApplicationModule SPA LinuxApp)
else(WIN32)
add_subdirectory(Win32)
target_link_libraries(ApplicationModule SPA Win32App)
endif()

