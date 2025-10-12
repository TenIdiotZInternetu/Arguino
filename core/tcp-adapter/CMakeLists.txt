project(arguino.tcp)

add_library(arguino.tcp STATIC
    TestAdapter.cpp
    TcpServer.hpp
    StateEncoder.cpp
    StateEncoder.hpp
)

target_include_directories(arguino.tcp PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
    target_link_libraries(Arguino.core PRIVATE wsock32 ws2_32)
endif()