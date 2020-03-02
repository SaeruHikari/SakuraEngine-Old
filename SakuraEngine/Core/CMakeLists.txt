add_subdirectory(CoreMinimal)
add_subdirectory(Math)
add_subdirectory(Containers)
add_subdirectory(EngineUtils)


add_library(Core Core.cpp)
add_library(CoreModule SHARED CoreModule.cpp)
target_link_libraries(Core EngineUtils)
target_link_libraries(CoreModule Core SPA)