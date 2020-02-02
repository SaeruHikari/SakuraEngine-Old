add_subdirectory(GraphicsCommon)
add_subdirectory(CGD_Vulkan)
add_subdirectory(CGD_Direct3D12)
add_subdirectory(CGD)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GraphicsCommon)

message(STATUS "GraphicsInterface Root: ${CMAKE_CURRENT_SOURCE_DIR}")