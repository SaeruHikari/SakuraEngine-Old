Module(
    NAME RuntimeCore
    TYPE Library
    SRC_PATH  #Default as Source
    DEPS
    DEPS_PUBLIC marl GSL
    INCLUDES_PUBLIC  
        /Include
    LINKS
    LINKS_PUBLIC
)

## Installed Includes: Include/${Scope}/${NAME}/${INCLUDES_PUBLIC}
## Solved Includes: Include/${Scope}/${DEP}/...