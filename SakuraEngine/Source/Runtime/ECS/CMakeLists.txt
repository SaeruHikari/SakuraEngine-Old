project(ECS)

Module(
    NAME ECS
    TYPE Library
    SRC_PATH  
        #Default as Source
    DEPS
    DEPS_PUBLIC 
        RuntimeCore 
        CMakeImported::DotsCodebase CMakeImported::DotsDatabase
    INCLUDES_PUBLIC 
        #Default as RuntimeCore
    LINKS
    LINKS_PUBLIC
)