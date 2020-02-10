find_file(SpecificTimerImpl GpuTimer.cpp *GpuTimer*.cpp 
    PATHS ${CMAKE_CURRENT_SOURCE_DIR})

if(SpecificTimerImpl)
else(SpecificTimerImpl)
set(SpecificTimerImpl ../CGD/GpuTimer.cpp)
endif(SpecificTimerImpl)

add_library(CGD_Vulkan 
    CGD_Vulkan.cpp  
    ../CGD/CommandContext.cpp  
    ../CGD/Profiling.cpp
    ${SpecificTimerImpl}
)