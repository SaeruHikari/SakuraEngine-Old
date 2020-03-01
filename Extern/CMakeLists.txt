add_subdirectory(include)
add_subdirectory(xxHash)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
else()
#add_subdirectory(_internal)
endif()