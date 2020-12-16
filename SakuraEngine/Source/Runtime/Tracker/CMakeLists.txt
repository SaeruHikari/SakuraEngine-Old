project(Tracker)

# Tracy Apps
download_and_extract(
    ${FILE_SERVER_DIR}/code/tracy-0.7.4.zip
    ${ENGINE_DIR}/Build/Code/tracy.zip
    ${CMAKE_CURRENT_SOURCE_DIR}/tracy-0.7.4
)
Module(
    NAME Tracker
    TYPE Library
    SRC_PATH  
        /Source
    DEPS 
        
    DEPS_PUBLIC 
        RuntimeCore 
    INCLUDES_PUBLIC 
        /Include
        /tracy-0.7.4
    LINKS
    LINKS_PUBLIC dxcompiler
)

if(SAKURA_HOST)
# Tracy Apps
download_and_extract(
    ${FILE_SERVER_DIR}/binaries/${SAKURA_PLATFORM}/${SAKURA_ARCH}/common/tracy-0.7.4-app.zip
    ${ENGINE_DIR}/Build/PreBuilt/Common/tracy-0.7.4-app.zip
    ${ENGINE_BIN_DIR}/Tools/
)
endif(SAKURA_HOST)