add_subdirectory(GraphicsCommon)
add_subdirectory(CGD_Vulkan)
add_subdirectory(CGD_Direct3D12)
add_subdirectory(CGD)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GraphicsCommon)

add_library(CGDModule SHARED CGDModule.h CGDModule.cpp)
target_link_libraries(CGDModule SPA CGD)
  
message(STATUS "Graphicsinterface Root: ${CMAKE_CURRENT_SOURCE_DIR}")