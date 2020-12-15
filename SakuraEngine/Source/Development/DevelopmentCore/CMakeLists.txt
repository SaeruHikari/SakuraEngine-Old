if(SAKURA_HOST)

project(DevelopmentCore)

Module(
    NAME DevelopmentCore
    TYPE Library
    SRC_PATH  
        #Default as Source
    DEPS
    DEPS_PUBLIC 
        RuntimeCore 
    INCLUDES_PUBLIC 
        #Default as RuntimeCore
    LINKS
    LINKS_PUBLIC
)

endif(SAKURA_HOST)