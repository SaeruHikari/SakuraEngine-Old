add_subdirectory(googletest)
add_subdirectory(include)

if(LINUX)
else()
add_subdirectory(_internal)
endif()