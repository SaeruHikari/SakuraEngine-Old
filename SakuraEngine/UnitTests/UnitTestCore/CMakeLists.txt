message(STATUS "Generating UnitTestCore.exe")
message(STATUS "Generating UnitTestCoreUtils.exe")

include_directories(
    ${PROJECT_SOURCE_DIR}/Extern/googletest/googletest/include
)
include_directories(
    ${PROJECT_SOURCE_DIR}/Extern/include
)
include_directories(
    ${PROJECT_SOURCE_DIR}/Extern/googletest/googlemock/include
)

add_executable(UnitTestCore UnitTestCore.cpp)
add_executable(UnitTestCoreUtils UnitTestCoreUtils.cpp)

target_link_libraries(UnitTestCore 
GTest::gtest GTest::gtest_main 
GTest::gmock GTest::gmock_main CoreModule)
target_link_libraries(UnitTestCoreUtils 
GTest::gtest GTest::gtest_main 
GTest::gmock GTest::gmock_main CoreModule)

add_test(UnitTestCore UnitTestCore)
