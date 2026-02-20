project(timer)

add_library(timer STATIC
        Timer.hpp
        Timer.cpp
)

target_include_directories(timer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)