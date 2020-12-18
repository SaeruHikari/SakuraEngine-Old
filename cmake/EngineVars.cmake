set(DUMP_BUILD_MESSAGE OFF)

set(ENGINE_DIR ${PROJECT_SOURCE_DIR}/SakuraEngine) 
set(ENGINE_SRC_DIR ${ENGINE_DIR}/Source) 
set(ENGINE_BIN_DIR ${ENGINE_DIR}/Binaries) 
set(ENGINE_TOOLS_DIR ${ENGINE_DIR}/Binaries/Tools) 
set(FILE_SERVER_DIR "http://139.59.27.159//SaeruHikari")
if(web)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ENGINE_BIN_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>$<$<CONFIG:MinSizeRel>:Release>)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ENGINE_BIN_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>$<$<CONFIG:MinSizeRel>:Release>)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ENGINE_BIN_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>$<$<CONFIG:MinSizeRel>:Release>)
endif()

link_directories(
    ${ENGINE_BIN_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>$<$<CONFIG:MinSizeRel>:Release>)
set(SAKURA_FINAL_BIN_DIR 
    ${ENGINE_BIN_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>$<$<CONFIG:MinSizeRel>:Release>)

