project(arguino.tcp)

add_library(arguino.tcp STATIC
    TestAdapter.cpp
        TcpServer.cpp
        TcpServer.hpp
)

if(WIN32)
    target_link_libraries(Arguino.core PRIVATE wsock32 ws2_32)
endif()