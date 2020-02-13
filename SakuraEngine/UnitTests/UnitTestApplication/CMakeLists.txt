message(STATUS "Generating UnitTestApplication.exe")
message(STATUS "Generating UnitTestWinApp.exe")
add_executable(UnitTestApplication UnitTestApplication.cpp)
add_executable(UnitTestWinApp UnitTestWinApp.cpp)

if(WIN32)
set(TARGET_APP_LIB "Win32App")
else(LINUX)
set(TARGET_APP_LIB "LinuxApp")
endif(WIN32)

target_link_libraries(UnitTestApplication gtest gtest_main Core ${TARGET_APP_LIB})

target_link_libraries(UnitTestWinApp Core user32 ${TARGET_APP_LIB})