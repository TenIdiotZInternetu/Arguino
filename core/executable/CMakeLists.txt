project(arguino.executable)

add_executable(arguino main.cpp
        sketch.cpp)

target_link_libraries(arguino PRIVATE arguino.simulator)