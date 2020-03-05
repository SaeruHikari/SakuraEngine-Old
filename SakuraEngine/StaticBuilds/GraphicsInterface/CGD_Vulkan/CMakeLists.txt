add_subdirectory(CommandObjects)

find_file(SpecificTimerImpl GpuTimer.cpp *GpuTimer*.cpp 
    PATHS ${CMAKE_CURRENT_SOURCE_DIR})

if(SpecificTimerImpl)
else(SpecificTimerImpl)
set(SpecificTimerImpl ../CGD/GpuTimer.cpp)
endif(SpecificTimerImpl)

add_library(CGD_Vulkan 
    CGD_Vulkan.cpp  
    CommandObjects/CommandContext_Vk.cpp 
    ../CGD/CommandContext.cpp  
    CommandObjects/CommandQueue_Vk.cpp  
    ../CGD/Profiling.cpp
    ${SpecificTimerImpl} 
)

target_link_libraries(CGD_Vulkan Core ${Vulkan_LIBRARY})