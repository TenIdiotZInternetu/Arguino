project(logger)

add_library(logger STATIC
        ILogger.hpp
        FileLogger.hpp
        FileLogger.cpp
)

target_include_directories(logger PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)