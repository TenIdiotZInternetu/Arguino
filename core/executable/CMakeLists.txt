project(arguino.executable)

find_package(Boost REQUIRED COMPONENTS program_options REQUIRED)

# TCP IMPLEMENTATION
add_executable(arguino-tcp.exe main.cpp)
target_compile_definitions(arguino-tcp.exe PRIVATE ARGUINO_TCP)
target_link_libraries(arguino-tcp.exe PRIVATE
    arguino.simulator
    arguino.tcp
    Boost::program_options
)

# SHARED MEMORY IMPLEMENTATION
add_executable(arguino-shared-memory.exe main.cpp)
target_compile_definitions(arguino-shared-memory.exe PRIVATE ARGUINO_SHARED_MEMORY)
target_link_libraries(arguino-shared-memory.exe PRIVATE
    arguino.simulator
    Boost::program_options
)

# DEFAULT EXE
set(DEFAULT_IMPL arguino-shared-memory.exe)
set_target_properties(${DEFAULT_IMPL} PROPERTIES OUTPUT_NAME arguino)
add_executable(arguino ALIAS ${DEFAULT_IMPL})