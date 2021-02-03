if(SAKURA_PLATFORM MATCHES "prospero")
else()
    set(MARL marl)
endif()

Module(
    NAME RuntimeCore
    TYPE Library
    SRC_PATH 
        /Source/Common
        /Source/Platform/${SAKURA_PLATFORM}
    DEPS
    DEPS_PUBLIC ${MARL} GSL DirectXMath SIMDJson
    INCLUDES_PUBLIC  
        /Include
    INCLUDES 
        ${LIBUUID_INCLUDE_DIR}
    LINKS
    LINKS_PUBLIC ${LIBUUID_LIBRARY}
)

# libUUID
if(WIN32)
    target_compile_definitions(RuntimeCore PRIVATE GUID_WINDOWS)
elseif(APPLE)
    find_library(CFLIB CoreFoundation)
    target_link_libraries(RuntimeCore ${CFLIB})
    target_compile_definitions(RuntimeCore PRIVATE GUID_CFUUID)
elseif(ANDROID)
    # GUID_ANDROID is used in the headers, so make PUBLIC
    target_compile_definitions(RuntimeCore PUBLIC GUID_ANDROID)
elseif(SAKURA_PLATFORM MATCHES "prospero")
  
else()
    find_package(Libuuid REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
            "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()
    target_link_libraries(RuntimeCore ${LIBUUID_LIBRARY})
    target_compile_definitions(RuntimeCore PRIVATE GUID_LIBUUID)
endif()

## Installed Includes: Include/${Scope}/${NAME}/${INCLUDES_PUBLIC}
## Solved Includes: Include/${Scope}/${DEP}/...