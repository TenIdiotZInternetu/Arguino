project(arguino.executable)

find_package(Boost REQUIRED COMPONENTS program_options REQUIRED)

if(WIN32)
    set(PREPROCESS_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/preprocess.bat")
else()
    set(PREPROCESS_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/preprocess.sh")
endif()

set(SKETCH "" CACHE PATH "Path to the sketch directory")
message("-- Setting sketch directory to: ${SKETCH}")

if(SKETCH)
    file(GLOB INO_FILES CONFIGURE_DEPENDS "${SKETCH}/*.ino")
    file(GLOB H_FILES CONFIGURE_DEPENDS "${SKETCH}/*.h")
    file(GLOB HPP_FILES CONFIGURE_DEPENDS "${SKETCH}/*.hpp")
    file(GLOB C_FILES CONFIGURE_DEPENDS "${SKETCH}/*.c")
    file(GLOB CPP_FILES CONFIGURE_DEPENDS "${SKETCH}/*.cpp")
    file(GLOB S_FILES CONFIGURE_DEPENDS "${SKETCH}/*.S")
endif()

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/sketch/sketch.cpp"
  COMMAND "${PREPROCESS_SCRIPT}" "${SKETCH}" "${CMAKE_CURRENT_SOURCE_DIR}"
  DEPENDS "${INO_FILES}" "${H_FILES}" "${HPP_FILES}" "${C_FILES}" "${CPP_FILES}" "${S_FILES}" "${PREPROCESS_SCRIPT}"
  COMMENT "Preprocessing sketch files from ${SKETCH}..."
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