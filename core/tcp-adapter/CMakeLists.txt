project(arguino.tcp)

if (NOT ${Boost_Found})
    find_package(Boost REQUIRED COMPONENTS asio)
    target_include_directories(arguino.tcp PUBLIC ${Boost_INCLUDE_DIRS})
endif()

add_library(arguino.tcp STATIC
    TcpServer.hpp
    ArguinoConnectionHandler.hpp
    StateEncoder.hpp
)

target_include_directories(arguino.tcp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(arguino.tcp PUBLIC
    arguino.simulator
    logger
)

if(WIN32)
    target_link_libraries(arguino.tcp PRIVATE wsock32 ws2_32)
endif()

