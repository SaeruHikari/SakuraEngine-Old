message(STATUS "Generating UnitTestApplication.exe")
add_executable(UnitTestApplication UnitTestApplication.cpp)
target_link_libraries(UnitTestApplication PRIVATE
GTest::gtest GTest::gtest_main 
GTest::gmock GTest::gmock_main 
Core ApplicationModule)

