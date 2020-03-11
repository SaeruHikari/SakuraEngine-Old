#Core Dispatcher of the Engine
add_library(EditorModule SHARED EditorModule.cpp)


if(WIN32)
add_subdirectory(Win32)
target_link_libraries(EditorModule SPA ApplicationModule CGD Win32Editor)
else()
add_subdirectory(Linux)
target_link_libraries(EditorModule SPA ApplicationModule CGD LinuxEditor)
endif()