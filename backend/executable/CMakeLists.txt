project(arguino.executable)

find_package(Boost REQUIRED COMPONENTS program_options REQUIRED)

# SKETCH PREPROCESSING
if(WIN32)
    set(PREPROCESS_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/preprocess.bat")
    set(PREPROCESS_COMMAND call)
else()
    set(PREPROCESS_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/preprocess.sh")
    set(PREPROCESS_COMMAND)
endif()

set(SKETCH "" CACHE STRING "Path to the sketch directory")

if(NOT SKETCH)
    message(FATAL_ERROR "Sketch directory not specified!")
endif()

get_filename_component(SKETCH "${SKETCH}" ABSOLUTE BASE_DIR "${CMAKE_SOURCE_DIR}")
get_filename_component(SKETCH_NAME "${SKETCH}" NAME)

message("-- Setting sketch directory to: ${SKETCH}")

file(GLOB INO_FILES CONFIGURE_DEPENDS "${SKETCH}/*.ino")
file(GLOB H_FILES CONFIGURE_DEPENDS "${SKETCH}/*.h")
file(GLOB HPP_FILES CONFIGURE_DEPENDS "${SKETCH}/*.hpp")
file(GLOB C_FILES CONFIGURE_DEPENDS "${SKETCH}/*.c")
file(GLOB CPP_FILES CONFIGURE_DEPENDS "${SKETCH}/*.cpp")
file(GLOB S_FILES CONFIGURE_DEPENDS "${SKETCH}/*.S")

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/sketch/sketch.cpp"
    COMMAND ${PREPROCESS_COMMAND} "${PREPROCESS_SCRIPT}" "${SKETCH}" "${CMAKE_CURRENT_SOURCE_DIR}"
  DEPENDS "${INO_FILES}" "${H_FILES}" "${HPP_FILES}" "${C_FILES}" "${CPP_FILES}" "${S_FILES}" "${PREPROCESS_SCRIPT}"
  COMMENT "Preprocessing sketch files from ${SKETCH}"
  VERBATIM
)

set_source_files_properties(main.cpp PROPERTIES 
    OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/sketch/sketch.cpp"
)

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

set_target_properties(arguino-tcp PROPERTIES OUTPUT_NAME "arguino-tcp-${SKETCH_NAME}")

# SHARED MEMORY IMPLEMENTATION
add_executable(arguino main.cpp
    ipc/shmemAdapter.cpp
    encoders/textEncoder.cpp
)

target_compile_definitions(arguino PRIVATE ARGUINO_SHARED_MEMORY)
target_link_libraries(arguino PRIVATE
    arguino.simulator
    arguino.shmem
    Boost::program_options
)

set_target_properties(arguino PROPERTIES OUTPUT_NAME "arguino-${SKETCH_NAME}")