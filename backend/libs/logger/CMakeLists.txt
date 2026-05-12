project(logger)

add_library(logger STATIC
        ILogger.hpp
        FileLogger.cpp
        Messages.hpp
)

target_include_directories(logger PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(logger PRIVATE
        timer
)