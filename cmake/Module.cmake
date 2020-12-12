include (CMakeParseArguments)

function(GlobCXXHeaders AbsPath OutFiles)
    file(GLOB_RECURSE 
        GlobFiles
        ${AbsPath}/*.h
        ${AbsPath}/*.hpp
        ${AbsPath}/*.hxx
        ${AbsPath}/*.inl
    )
    set(${OutFiles} ${GlobFiles} PARENT_SCOPE)
endfunction()

function(GlobCXXFiles AbsPath OutFiles)
    file(GLOB_RECURSE 
        GlobFiles 
        ${AbsPath}/*.cc
        ${AbsPath}/*.cpp
        ${AbsPath}/*.c
        ${AbsPath}/*.cxx
        ${AbsPath}/*.h
        ${AbsPath}/*.hpp
        ${AbsPath}/*.hxx
        ${AbsPath}/*.inl

        ${AbsPath}/*.natvis
    )
    set(${OutFiles} ${GlobFiles} PARENT_SCOPE)
endfunction()



function(Module)
    CMAKE_PARSE_ARGUMENTS(  
        MODULE "" "NAME;TYPE;"  
        "DEPS;DEPS_PUBLIC;INCLUDES_PUBLIC;INCLUDES;SRC_PATH;LINKS;LINKS_PUBLIC"  
        ${ARGN}  
    )
    get_property(CurrentScope GLOBAL PROPERTY Scope)

    list(LENGTH MODULE_SRC_PATH _length) 
    if(${_length} LESS 1)
        set(MODULE_SRC_PATH Source)
    endif()
    list(LENGTH MODULE_INCLUDES_PUBLIC _length2) 
    if(${_length2} LESS 1)
        set(MODULE_INCLUDES_PUBLIC Include)
    endif()
    list(LENGTH MODULE_INCLUDES _length3) 
    if(${_length3} LESS 1)
        set(MODULE_INCLUDES ${MODULE_SRC_PATH})
    endif()

    foreach(p ${MODULE_SRC_PATH})
        GlobCXXFiles(${ENGINE_DIR}/Source/${CurrentScope}/${MODULE_NAME}/${p} MODULE_SRC_T)
        set(MODULE_SRC ${MODULE_SRC} ${MODULE_SRC_T})
        message(STATUS ${ENGINE_DIR}/Source/${CurrentScope}/${MODULE_NAME}/${p})
    endforeach()

    foreach(p ${MODULE_INCLUDES_PUBLIC})
        GlobCXXFiles(${ENGINE_DIR}/Source/${CurrentScope}/${MODULE_NAME}/${p} MODULE_SRC_T)

        set(MODULE_ABS_HEADER_DIR ${MODULE_ABS_HEADER_DIR} ${ENGINE_DIR}/Source/${CurrentScope}/${MODULE_NAME}/${p})
        set(MODULE_REL_HEADER_DIR ${MODULE_REL_HEADER_DIR} Source/${CurrentScope}/${MODULE_NAME}/${p})
        set(MODULE_SRC ${MODULE_SRC} ${MODULE_SRC_T})
    endforeach()


    if(${MODULE_TYPE} STREQUAL "Library")
        add_library(${MODULE_NAME} SHARED ${MODULE_SRC})
        target_compile_definitions(${MODULE_NAME} 
            PRIVATE 
            ${MODULE_NAME}API=${API_EXPORT_DEF}
            ${MODULE_NAME}HIDDEN=${API_HIDDEN_DEF}
        )
        add_library(SakuraEngine::${MODULE_NAME} ALIAS ${MODULE_NAME})
    elseif(${MODULE_TYPE} STREQUAL "Executable")
        add_executable(${MODULE_NAME} ${MODULE_SRC})
        add_executable(SakuraEngine::${MODULE_NAME} ALIAS ${MODULE_NAME})
    elseif(${MODULE_TYPE} STREQUAL "Test")
        add_executable(${MODULE_NAME} ${MODULE_SRC})
        add_executable(SakuraEngine::${MODULE_NAME} ALIAS ${MODULE_NAME})
    endif(${MODULE_TYPE} STREQUAL "Library")

    #target_sources(${MODULE_NAME} PRIVATE ${MODULE_SRC})
    target_include_directories(${MODULE_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${MODULE_ABS_HEADER_DIR}>
        $<INSTALL_INTERFACE:${MODULE_REL_HEADER_DIR}>
        PRIVATE
        ${MODULE_INCLUDES}
    )

    target_link_libraries(${MODULE_NAME}
        PUBLIC
        ${MODULE_DEPS_PUBLIC}
        ${MODULE_LINKS_PUBLIC}
        PRIVATE
        ${MODULE_DEPS}
        ${MODULE_LINKS}
    )
    foreach(dep ${MODULE_DEPS_PUBLIC})
        target_compile_definitions(${MODULE_NAME} 
            PUBLIC 
            ${dep}API=${API_IMPORT_DEF}
            ${dep}HIDDEN=${API_HIDDEN_DEF}
        )
    endforeach()
    foreach(private_dep ${MODULE_DEPS})
        target_compile_definitions(${MODULE_NAME} 
            PRIVATE 
            ${private_dep}API=${API_IMPORT_DEF}
            ${private_dep}HIDDEN=${API_HIDDEN_DEF}
        )
    endforeach()

    install(TARGETS ${MODULE_NAME}
        EXPORT "SakuraEngine${CurrentScope}"
        RUNTIME DESTINATION "/Binaries"
        ARCHIVE DESTINATION "/Lib"
        LIBRARY DESTINATION "/Lib"
    )

    set_property(GLOBAL PROPERTY ${CurrentScope}HasModule TRUE)
endfunction(Module)


function(CMakeImportedModule)
    CMAKE_PARSE_ARGUMENTS(  
        MODULE "" "NAME;TYPE;SELF_INSTALL;"  
        "DEPS;DEPS_PUBLIC;INCLUDES_PUBLIC;INCLUDES;SRC_PATH;LINKS;LINKS_PUBLIC"  
        ${ARGN}  
    )
    get_property(CurrentScope GLOBAL PROPERTY Scope)

    if(NOT ${MODULE_SELF_INSTALL})
        install(TARGETS ${MODULE_NAME}
            EXPORT "SakuraEngine${CurrentScope}CMakeImported"
            RUNTIME DESTINATION "/Binaries"
            ARCHIVE DESTINATION "/Lib"
            LIBRARY DESTINATION "/Lib"
            INCLUDES DESTINATION ${MODULE_INCLUDES_PUBLIC}
        )
    else()
        set_property(GLOBAL PROPERTY ${CurrentScope}HasCMakeImported TRUE)
    endif()
    add_library(CMakeImported::${MODULE_NAME} ALIAS ${MODULE_NAME})
endfunction()