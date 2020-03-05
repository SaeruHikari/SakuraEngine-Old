include_directories( ${PROJECT_SOURCE_DIR}/GraphicsInterface/CGD_Direct3D12)
include_directories( ${PROJECT_SOURCE_DIR}/GraphicsInterface/Vulkan)
include_directories( ${PROJECT_SOURCE_DIR}/GraphicsInterface/GraphicsCommon)

add_library(CGD 
SHARED
CGD.cpp 
../GraphicsCommon/CGD.h)

target_link_libraries(CGD CGD_Vulkan CGD_Direct3D12)
