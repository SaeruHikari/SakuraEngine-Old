#Core Dispatcher of the Engine
add_library(EditorModule SHARED EditorModule.cpp)

if(${CMAKE_SYSTEM} MATCHES "Linux")
add_subdirectory(Linux)
target_link_libraries(EditorModule SPA ApplicationModule CGDModule LinuxEditor)
elseif(WIN32)
add_subdirectory(Win32)
target_link_libraries(EditorModule SPA ApplicationModule CGDModule Win32Editor)
endif()