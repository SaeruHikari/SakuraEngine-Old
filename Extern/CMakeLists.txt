add_subdirectory(include)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
else()
#add_subdirectory(_internal)
endif()