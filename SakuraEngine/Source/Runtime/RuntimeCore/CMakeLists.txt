project(RuntimeCore VERSION 0.0.1)

if(__COMPILER_PS5)
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
    DEPS_PUBLIC ${MARL} GSL crossguid
    INCLUDES_PUBLIC  
        /Include
    LINKS
    LINKS_PUBLIC
)

## Installed Includes: Include/${Scope}/${NAME}/${INCLUDES_PUBLIC}
## Solved Includes: Include/${Scope}/${DEP}/...