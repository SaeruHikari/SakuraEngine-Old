if(SAKURA_PLATFORM MATCHES "prospero")
else()
    set(MARL marl)
endif()

if(WIN32)
elseif(APPLE)
    find_library(LIBUUID_LIBRARY CoreFoundation)
    find_library(CORE_VIDEO CoreVideo)
    find_library(APP_KIT AppKit)
    set(CORE_LIBS ${CORE_LIBS} ${CORE_VIDEO} ${APP_KIT})
elseif(ANDROID)
elseif(SAKURA_PLATFORM MATCHES "prospero")
else()
    find_package(LIBUUID_LIBRARY REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
            "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()
endif()
set(CORE_LIBS ${CORE_LIBS} ${LIBUUID_LIBRARY})


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
    LINKS_PUBLIC ${CORE_LIBS}
)

# libUUID
if(WIN32)
    target_compile_definitions(RuntimeCore PRIVATE GUID_WINDOWS)
elseif(APPLE)
    target_compile_definitions(RuntimeCore PRIVATE GUID_CFUUID)
elseif(ANDROID)
    target_compile_definitions(RuntimeCore PUBLIC GUID_ANDROID)
elseif(SAKURA_PLATFORM MATCHES "prospero")
  
else()
    target_compile_definitions(RuntimeCore PRIVATE GUID_LIBUUID)
endif()

## Installed Includes: Include/${Scope}/${NAME}/${INCLUDES_PUBLIC}
## Solved Includes: Include/${Scope}/${DEP}/...