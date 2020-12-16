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

function(SourceGroupByDir)
    CMAKE_PARSE_ARGUMENTS(  
        m "" "cur_dir"  
        "src_files"  
        ${ARGN}  
    )
    foreach(sgbd_file ${m_src_files})
        string(REGEX REPLACE ${m_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
        string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
        string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
        if(sgbd_nogroup)
            set(sgbd_group_name "\\")
        endif(sgbd_nogroup)
        source_group(${sgbd_group_name} FILES ${sgbd_file})
    endforeach(sgbd_file)
endfunction(SourceGroupByDir)



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

    get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    foreach(p ${MODULE_SRC_PATH})
        GlobCXXFiles(${ENGINE_DIR}/Source/${CurrentScope}/${ProjectId}/${p} MODULE_SRC_T)
        set(MODULE_SRC ${MODULE_SRC} ${MODULE_SRC_T})
    endforeach()

    foreach(p ${MODULE_INCLUDES_PUBLIC})
        GlobCXXHeaders(${ENGINE_DIR}/Source/${CurrentScope}/${ProjectId}/${p} MODULE_SRC_T)

        set(MODULE_ABS_HEADER_DIR ${MODULE_ABS_HEADER_DIR} ${ENGINE_DIR}/Source/${CurrentScope}/${ProjectId}/${p})
        set(MODULE_REL_HEADER_DIR ${MODULE_REL_HEADER_DIR} Source/${CurrentScope}/${ProjectId}/${p})
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

    target_include_directories(${MODULE_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${MODULE_ABS_HEADER_DIR}>"
        "$<INSTALL_INTERFACE:${MODULE_REL_HEADER_DIR}>"
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
        RUNTIME DESTINATION "${ENGINE_DIR}/Installed/Binaries"
        ARCHIVE DESTINATION "${ENGINE_DIR}/Installed/Lib"
        LIBRARY DESTINATION "${ENGINE_DIR}/Installed/Lib"
    )

    if(MSVC)
        # Enable MSVC UnityBuild
        set_target_properties(${MODULE_NAME} PROPERTIES VS_GLOBAL_EnableUnitySupport true)
        SourceGroupByDir(
            cur_dir
            ${ENGINE_DIR}/Source/${CurrentScope}/${ProjectId}
            src_files
            ${MODULE_SRC}
        )
        set_property(TARGET "${MODULE_NAME}" PROPERTY FOLDER "${CurrentScope}/${ProjectId}")
    endif(MSVC)

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
            RUNTIME DESTINATION "${ENGINE_DIR}/Installed/Binaries"
            ARCHIVE DESTINATION "${ENGINE_DIR}/Installed/Lib"
            LIBRARY DESTINATION "${ENGINE_DIR}/Installed/Lib"
            INCLUDES DESTINATION ${MODULE_INCLUDES_PUBLIC}
        )
    else()
        set_property(GLOBAL PROPERTY ${CurrentScope}HasCMakeImported TRUE)
    endif()
    add_library(CMakeImported::${MODULE_NAME} ALIAS ${MODULE_NAME})

    if(MSVC)
        set_property(TARGET "${MODULE_NAME}" PROPERTY FOLDER "${CurrentScope}/CMakeImported/${ProjectId}")
    endif(MSVC)
endfunction()