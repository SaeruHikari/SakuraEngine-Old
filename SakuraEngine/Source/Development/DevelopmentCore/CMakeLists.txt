if(SAKURA_HOST)

Module(
    NAME DevelopmentCore
    TYPE Library
    SRC_PATH  
        #Default as Source
    DEPS
    DEPS_PUBLIC 
        RuntimeCore 
    INCLUDES_PUBLIC 
        #
    LINKS
    LINKS_PUBLIC
)

endif(SAKURA_HOST)