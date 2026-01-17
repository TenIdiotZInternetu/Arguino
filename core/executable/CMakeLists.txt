project(arguino.executable)

add_executable(arguino main.cpp
    
)

target_link_libraries(arguino PRIVATE
    arguino.simulator
    arguino.tcp
    ulibpp
)