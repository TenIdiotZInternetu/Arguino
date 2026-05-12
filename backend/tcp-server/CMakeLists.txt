project(arguino.tcp)

find_package(Boost REQUIRED)

add_library(arguino.tcp STATIC
    TcpServer.cpp
    ConnectionHandler.cpp
)

target_include_directories(arguino.tcp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(arguino.tcp PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(arguino.tcp PUBLIC
    arguino.simulator
    logger
)

if(WIN32)
    target_link_libraries(arguino.tcp PRIVATE wsock32 ws2_32)
endif()

