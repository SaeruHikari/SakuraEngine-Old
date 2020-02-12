message(STATUS "Generating UnitTestApplication.exe")
add_executable(UnitTestApplication UnitTestApplication.cpp)

if(WIN32)
set(TARGET_APP_LIB "Win32App")
else(LINUX)
set(TARGET_APP_LIB "LinuxApp")
endif(WIN32)

target_link_libraries(UnitTestApplication gtest gtest_main 
	Core ${TARGET_APP_LIB})