project(arguino.executable)

find_package(Boost REQUIRED COMPONENTS program_options REQUIRED)

# TCP IMPLEMENTATION
add_executable(arguino-tcp main.cpp
    ipc/tcpAdapter.cpp
    encoders/textEncoder.cpp
)

target_compile_definitions(arguino-tcp PRIVATE ARGUINO_TCP)
target_link_libraries(arguino-tcp PRIVATE
    arguino.simulator
    arguino.tcp
    Boost::program_options
)

# SHARED MEMORY IMPLEMENTATION
add_executable(arguino-shared-memory main.cpp
    ipc/shmemAdapter.cpp
    encoders/textEncoder.cpp
)

target_compile_definitions(arguino-shared-memory PRIVATE ARGUINO_SHARED_MEMORY)
target_link_libraries(arguino-shared-memory PRIVATE
    arguino.simulator
    arguino.shmem
    Boost::program_options
)

# DEFAULT EXE
set(DEFAULT_IMPL arguino-shared-memory)
set_target_properties(${DEFAULT_IMPL} PROPERTIES OUTPUT_NAME arguino)
add_executable(arguino ALIAS ${DEFAULT_IMPL})