cmake_minimum_required(VERSION 3.18.0)

INCLUDE(cmake/Module.cmake)
INCLUDE(cmake/Initialize.cmake)

project(SakuraEngine VERSION 0.0.2)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/SakuraEngine)
include_directories(${ENGINE_DIR}/Source/External)

# Build all engine modules
FILE(GLOB children RELATIVE ${ENGINE_DIR}/Source/ ${ENGINE_DIR}/Source/*)
FOREACH(child ${children})
IF(IS_DIRECTORY ${ENGINE_DIR}/Source/${child})
    FILE(GLOB plugins RELATIVE ${ENGINE_DIR}/Source/${child} ${ENGINE_DIR}/Source/${child}/*)
    FOREACH(plugin ${plugins})    
    IF(IS_DIRECTORY ${ENGINE_DIR}/Source/${child}/${plugin})
            FILE(GLOB modules RELATIVE ${ENGINE_DIR}/Source/${child}/${plugin} ${ENGINE_DIR}/Source/${child}/${plugin}/*)
            IF(IS_DIRECTORY ${ENGINE_DIR}/Source/${child}/${plugin})
                if(EXISTS "${ENGINE_DIR}/Source/${child}/${plugin}/CMakeLists.txt")
                    set_property(GLOBAL PROPERTY Scope ${child})
                    set(Scopes ${Scopes} ${child})
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
    install(EXPORT SakuraEngine${scope}
        FILE SakuraEngine${scope}Targets.cmake
        NAMESPACE SakuraEngine::${scope}::
        DESTINATION cmake/${scope}
    )
endforeach()

configure_file(SakuraEngineConfig.cmake.in SakuraEngineConfig.cmake @ONLY)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/SakuraEngineConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SakuraEngineConfigVersion.cmake"
    DESTINATION cmake
)