project(arguino.shmem)

find_package(Boost REQUIRED)

add_library(arguino.shmem STATIC
    TwoWayBuffer.cpp
    MemoryRegion.cpp
    CircularBuffer.cpp
)
    
target_include_directories(arguino.shmem PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(arguino.shmem PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(arguino.shmem PUBLIC
    logger
)