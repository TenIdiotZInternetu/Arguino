cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)

project(arguino.core)

add_subdirectory(libs)
add_subdirectory(tcp-server)
add_subdirectory(shared-memory)
add_subdirectory(simulator)
add_subdirectory(executable)