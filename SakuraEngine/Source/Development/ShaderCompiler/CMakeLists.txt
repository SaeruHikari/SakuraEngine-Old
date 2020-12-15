if(SAKURA_HOST)

project(ShaderCompiler)

download_and_extract(
    ${FILE_SERVER_DIR}/binaries/${SAKURA_PLATFORM}/x64/common/dxc.zip
    ${ENGINE_DIR}/Build/PreBuilt/Common/dxc.zip
    ${ENGINE_BIN_DIR}/Release/
)
file(ARCHIVE_EXTRACT 
    INPUT 
        ${ENGINE_DIR}/Build/PreBuilt/Common/dxc.zip 
    DESTINATION ${ENGINE_BIN_DIR}/Debug/
)

Module(
    NAME ShaderCompiler
    TYPE Library
    SRC_PATH  
        #Default as Source
    DEPS 
        RenderGraph
    DEPS_PUBLIC 
        RuntimeCore DevelopmentCore
    INCLUDES_PUBLIC 
        #Default as RuntimeCore
    LINKS
    LINKS_PUBLIC dxcompiler
)

endif(SAKURA_HOST)