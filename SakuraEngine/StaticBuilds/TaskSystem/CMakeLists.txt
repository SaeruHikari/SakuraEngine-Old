add_library(TaskSystem STATIC TaskSystem.cpp)

target_link_libraries(TaskSystem PRIVATE Cpp-Taskflow::Cpp-Taskflow)