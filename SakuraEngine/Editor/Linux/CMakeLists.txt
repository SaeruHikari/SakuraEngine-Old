add_library(LinuxEditor LinuxEditor.cpp)

add_subdirectory(Execute)

target_link_libraries(LinuxEditor CoreModule)