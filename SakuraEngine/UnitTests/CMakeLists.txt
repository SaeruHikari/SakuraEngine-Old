add_subdirectory(UnitTestGraphics)
add_subdirectory(UnitTestCore)

#GTest Support
include_directories(
    ${PROJECT_SOURCE_DIR}/Extern/googletest/googletest/include
)
include_directories(
    ${PROJECT_SOURCE_DIR}/Extern/googletest/googlemock/include
)

link_libraries(Core)