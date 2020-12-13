cmake_minimum_required(VERSION 3.18.0)

project(SakuraEngine VERSION 0.0.2)

include(cmake/EngineVars.cmake)
INCLUDE(cmake/Network.cmake)
INCLUDE(cmake/Module.cmake)
INCLUDE(cmake/Initialize.cmake)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/SakuraEngine)
set(CMAKE_INSTALL_DATADIR ${PROJECT_SOURCE_DIR}/SakuraEngine)
set(CMAKE_INSTALL_INCLUDEDIR ${PROJECT_SOURCE_DIR}/SakuraEngine/Extern)
include_directories(${ENGINE_DIR}/Extern)

# Build all engine modules
FILE(GLOB children RELATIVE ${ENGINE_DIR}/Source/ ${ENGINE_DIR}/Source/*)
FOREACH(child ${children})
IF(IS_DIRECTORY ${ENGINE_DIR}/Source/${child})

    set_property(GLOBAL PROPERTY Scope ${child})
    set(Scopes ${Scopes} ${child})

    FILE(GLOB plugins RELATIVE ${ENGINE_DIR}/Source/${child} ${ENGINE_DIR}/Source/${child}/*)
    FOREACH(plugin ${plugins})    
    IF(IS_DIRECTORY ${ENGINE_DIR}/Source/${child}/${plugin})
            FILE(GLOB modules RELATIVE ${ENGINE_DIR}/Source/${child}/${plugin} ${ENGINE_DIR}/Source/${child}/${plugin}/*)
            IF(IS_DIRECTORY ${ENGINE_DIR}/Source/${child}/${plugin})
                if(EXISTS "${ENGINE_DIR}/Source/${child}/${plugin}/CMakeLists.txt")
                    add_subdirectory(${ENGINE_DIR}/Source/${child}/${plugin})
                endif()
            ENDIF()
    ENDIF()
    ENDFOREACH()
    
ENDIF()
ENDFOREACH()


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SakuraEngineConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion  
)

# Deal with Scope Exports
foreach(scope ${Scopes})
    get_property(HasCMakeImported GLOBAL PROPERTY ${scope}HasCMakeImported)
    get_property(HasModule GLOBAL PROPERTY ${scope}HasModule)
    if(${HasCMakeImported})
        install(EXPORT SakuraEngine${scope}CMakeImported
            FILE SakuraEngine${scope}TargetsCMakeImported.cmake
            NAMESPACE SakuraEngine::${scope}::CMakeImported::
            DESTINATION cmake/${scope}
        )
    endif()

    if(${HasModule})
        install(EXPORT SakuraEngine${scope}
            FILE SakuraEngine${scope}Targets.cmake
            NAMESPACE SakuraEngine::${scope}::
            DESTINATION cmake/${scope}
        )
    endif()
endforeach()


configure_file(SakuraEngineConfig.cmake.in SakuraEngineConfig.cmake @ONLY)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/SakuraEngineConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SakuraEngineConfigVersion.cmake"
    DESTINATION cmake
)