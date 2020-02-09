add_subdirectory(UnitTestGraphics)
add_subdirectory(UnitTestCore)
add_subdirectory(UnitTestApplication)
#GTest Support
include_directories(
    ${PROJECT_SOURCE_DIR}/Extern/googletest/googletest/include
)
include_directories(
    ${PROJECT_SOURCE_DIR}/Extern/googletest/googlemock/include
)