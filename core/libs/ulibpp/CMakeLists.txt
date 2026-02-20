# root cmake

cmake_minimum_required (VERSION 3.30)

project(ulibpp)

# trying to set the compiler standard
include(CheckCXXCompilerFlag)

# Check for standard to use
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
	check_cxx_compiler_flag(/std:c++17 HAVE_FLAG_STD_CXX17)
	check_cxx_compiler_flag(/std:c++14 HAVE_FLAG_STD_CXX14)
	check_cxx_compiler_flag(/std:c++11 HAVE_FLAG_STD_CXX11)
else ()
	check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)
	check_cxx_compiler_flag(-std=c++1z HAVE_FLAG_STD_CXX1Z)
	check_cxx_compiler_flag(-std=c++14 HAVE_FLAG_STD_CXX14)
	check_cxx_compiler_flag(-std=c++1y HAVE_FLAG_STD_CXX1Y)
	check_cxx_compiler_flag(-std=c++11 HAVE_FLAG_STD_CXX11)
	check_cxx_compiler_flag(-std=c++0x HAVE_FLAG_STD_CXX0X)
endif ()
if(HAVE_FLAG_STD_CXX17 OR HAVE_FLAG_STD_CXX1Z)
	set(ULIBPP_CXX_STD 17)
elseif(HAVE_FLAG_STD_CXX14 OR HAVE_FLAG_STD_CXX1Y)
	set(ULIBPP_CXX_STD 14)
elseif(HAVE_FLAG_STD_CXX11 OR HAVE_FLAG_STD_CXX0X)
	set(ULIBPP_CXX_STD 11)
else()
	message(FATAL_ERROR "Supported compilers must have at least support for C++11")
endif()

find_program(LSB_RELEASE_EXEC lsb_release)
execute_process(COMMAND ${LSB_RELEASE_EXEC} -is
    OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# create output name with suffix
function(create_suffix tgt resname)
	if(NOT CMAKE_BUILD_TYPE)
		message(FATAL_ERROR "No configuration selected")
	endif()
	# extract compiler's major version
	string(REGEX MATCH "[0-9]+" cxxmv ${CMAKE_CXX_COMPILER_VERSION})
	set(${resname} "${tgt}.${LSB_RELEASE_ID_SHORT}.${CMAKE_CXX_COMPILER_ID}${cxxmv}.${CMAKE_BUILD_TYPE}" PARENT_SCOPE)
endfunction()

add_subdirectory(src)
