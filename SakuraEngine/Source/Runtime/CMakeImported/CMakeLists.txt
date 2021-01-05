if(SAKURA_PLATFORM MATCHES "prospero")
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
    NAME Dots
    SELF_INSTALL FALSE
    INCLUDES_PUBLIC
        Source/Runtime/CMakeImported/Dots/Dots/include
        Source/Runtime/CMakeImported/Dots/Dots
)

CMakeImportedModule(
    NAME DotsRuntime
    SELF_INSTALL FALSE
    INCLUDES_PUBLIC
        Source/Runtime/CMakeImported/Dots/DotsRuntime
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