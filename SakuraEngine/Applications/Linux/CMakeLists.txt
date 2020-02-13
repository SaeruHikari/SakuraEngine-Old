if(LINUX)
add_library(LinuxApp LinuxEngine.cc LinuxWindow.cc)

find_library(XCB xcb)
message(STATUS ${XCB})
target_link_libraries(LinuxApp xcb Core)
endif(LINUX)