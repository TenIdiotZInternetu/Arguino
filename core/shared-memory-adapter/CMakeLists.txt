project(arguino.shmem)

if (NOT ${Boost_Found})
    find_package(Boost REQUIRED COMPONENTS interprocess)
    target_include_directories(arguino.shmem PUBLIC ${Boost_INCLUDE_DIRS})
endif()

add_library(arguino.shmem STATIC
    ShmemHandler.cpp
    ShmemWriter.cpp
)

target_include_directories(arguino.shmem PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(arguino.shmem PUBLIC
    arguino.simulator
    logger
)