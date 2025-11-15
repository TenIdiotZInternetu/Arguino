project(arguino.tcp)

if (NOT ${Boost_Found})
    find_package(Boost REQUIRED COMPONENTS asio)
    target_include_directories(arguino.tcp PUBLIC ${Boost_INCLUDE_DIRS})
endif()

add_library(arguino.tcp STATIC
    TestAdapter.cpp
    TcpServer.hpp
)

target_include_directories(arguino.tcp PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)


