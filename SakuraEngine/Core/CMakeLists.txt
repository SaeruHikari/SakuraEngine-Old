add_subdirectory(CoreMinimal)
add_subdirectory(Math)
add_subdirectory(Containers)
add_subdirectory(EngineUtils)


add_library(Core Core.cpp)
target_link_libraries(Core EngineUtils)