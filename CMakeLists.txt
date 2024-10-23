cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)

project(Arguino)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(Arguino main.cpp
        TCPConnection.cpp
        TCPConnection.hpp)

target_link_libraries(Arguino ${Boost_LIBRARIES})

if(WIN32)
    target_link_libraries(Arguino PRIVATE wsock32 ws2_32)
endif()
