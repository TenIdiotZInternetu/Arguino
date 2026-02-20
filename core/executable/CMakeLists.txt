project(arguino.executable)

find_package(Boost REQUIRED COMPONENTS program_options REQUIRED)

add_executable(arguino main.cpp)

target_link_libraries(arguino PRIVATE
    arguino.simulator
    arguino.tcp
    Boost::program_options
)