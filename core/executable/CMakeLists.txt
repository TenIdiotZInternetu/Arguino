project(arguino.executable)

add_executable(arguino main.cpp
    ArguinoConnectionHandler.hpp
    StateEncoder.hpp
)

target_link_libraries(arguino PRIVATE
    arguino.simulator
    arguino.tcp
    ulibpp
)

if(WIN32)
    target_link_libraries(arguino PRIVATE wsock32 ws2_32)
endif()