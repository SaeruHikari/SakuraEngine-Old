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

target_link_libraries(UnitTestCore gtest gtest_main Core)
target_link_libraries(UnitTestCoreUtils gtest gtest_main Core ${CONAN_LIBS})