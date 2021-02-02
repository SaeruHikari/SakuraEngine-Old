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
    DEPS_PUBLIC ${MARL} GSL crossguid DirectXMath
    INCLUDES_PUBLIC  
        /Include
    LINKS
    LINKS_PUBLIC
)

## Installed Includes: Include/${Scope}/${NAME}/${INCLUDES_PUBLIC}
## Solved Includes: Include/${Scope}/${DEP}/...