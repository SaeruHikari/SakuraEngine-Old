include_directories( ${PROJECT_SOURCE_DIR}/Graphicsinterface/CGD_Direct3D12)
include_directories( ${PROJECT_SOURCE_DIR}/Graphicsinterface/Vulkan)
include_directories( ${PROJECT_SOURCE_DIR}/Graphicsinterface/GraphicsCommon)

add_library(CGD 
CGD.cpp 
../GraphicsCommon/CGD.h)

target_link_libraries(CGD CGD_Vulkan CGD_Direct3D12)
