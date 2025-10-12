project(arguino.simulator)

add_library(arguino.simulator STATIC
        Arduino.cpp
        ArduinoState.cpp
        ../tcp-adapter/ArguinoConnectionHandler.cpp
        ../tcp-adapter/ArguinoConnectionHandler.hpp
)

target_link_libraries(arguino.simulator PRIVATE arguino.tcp)

target_include_directories(arguino.simulator PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)