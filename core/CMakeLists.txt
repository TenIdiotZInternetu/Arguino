cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)

project(arguino.core)

add_subdirectory(ulibpp)
add_subdirectory(tcp-adapter)
add_subdirectory(simulator)
add_subdirectory(executable)
add_subdirectory(timer)