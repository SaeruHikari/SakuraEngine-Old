add_library(ApplicationModule SHARED ApplicationModule.cpp)

target_link_libraries(ApplicationModule SPA SDL2::SDL2 SDL2::SDL2main fmt::fmt)

