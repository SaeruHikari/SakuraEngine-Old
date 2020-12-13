if(__COMPILER_PS5)
else()
    add_subdirectory(marl)
    CMakeImportedModule(
        NAME marl
        SELF_INSTALL FALSE
        INCLUDES_PUBLIC
            Source/Runtime/CMakeImported/marl/include
    )
endif()

add_subdirectory(Dots)

CMakeImportedModule(
    NAME DotsCodebase
    SELF_INSTALL FALSE
    INCLUDES_PUBLIC
        Source/Runtime/CMakeImported/Dots/Codebase/include
        Source/Runtime/CMakeImported/Dots/Database
        Source/Runtime/CMakeImported/Dots
)
CMakeImportedModule(
    NAME DotsDatabase
    SELF_INSTALL FALSE
    INCLUDES_PUBLIC
        Source/Runtime/CMakeImported/Dots
)


add_subdirectory(GSL)
CMakeImportedModule(
    NAME GSL
    SELF_INSTALL TRUE
)

add_subdirectory(crossguid)
CMakeImportedModule(
    NAME crossguid
    SELF_INSTALL FALSE
    INCLUDES_PUBLIC
        Source/Runtime/CMakeImported/crossguid/include
)