project(arguino.simulator)

add_library(arguino.simulator STATIC
        Arduino.cpp
        ArduinoState.cpp
)

target_link_libraries(arguino.simulator PUBLIC
        timer
        logger
)

target_include_directories(arguino.simulator PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)