# ulibpp cmake
set(sources upp_file.cpp upp_mthr.cpp upp_shrmem.cpp)
add_library(ulibpp STATIC ${sources})
target_include_directories(ulibpp PUBLIC "../h")
create_suffix(ulibpp outname)
message(STATUS "Output name ${outname}")
set_target_properties(ulibpp
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
	CXX_STANDARD ${ULIBPP_CXX_STD}
	CXX_EXTENSIONS OFF
	OUTPUT_NAME ${outname}
)
export(TARGETS ulibpp NAMESPACE ulibpp:: FILE "${CMAKE_SOURCE_DIR}/lib/cmake/ulibpp${CMAKE_BUILD_TYPE}Config.cmake")
