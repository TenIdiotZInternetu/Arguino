project(arguino.tcp)

find_package(Boost REQUIRED COMPONENTS asio)

add_library(arguino.tcp STATIC
    TestAdapter.cpp
    TcpServer.hpp
)

target_include_directories(arguino.tcp PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(arguino.tcp PUBLIC ${Boost_INCLUDE_DIRS})

