cmake_minimum_required(VERSION 3.20)

# Compiler selection (default/gcc/clang/msvc). Effective only at first configure.
set(LIBCPPRIME_COMPILER "default" CACHE STRING "Select C++ compiler: default/gcc/clang/msvc")
set_property(CACHE LIBCPPRIME_COMPILER PROPERTY STRINGS default gcc clang msvc)

# Generator compatibility check (gcc/clang cannot be used with Visual Studio generator)
if(LIBCPPRIME_COMPILER MATCHES "gcc|clang" AND CMAKE_GENERATOR MATCHES "Visual Studio")
	message(FATAL_ERROR
		"LIBCPPRIME_COMPILER=${LIBCPPRIME_COMPILER} but generator '${CMAKE_GENERATOR}' enforces MSVC.\n"
		"Use a Makefile/Ninja generator instead, e.g.:\n"
		"  Ninja: cmake -S . -B build-gcc -G \"Ninja\" -DLIBCPPRIME_COMPILER=gcc\n"
		"  MinGW: cmake -S . -B build-gcc -G \"MinGW Makefiles\" -DLIBCPPRIME_COMPILER=gcc"
	)
endif()

if(NOT DEFINED CMAKE_CXX_COMPILER)
	if(LIBCPPRIME_COMPILER STREQUAL "gcc")
		find_program(_GXX NAMES g++)
		if(NOT _GXX)
			message(FATAL_ERROR "g++ not found in PATH")
		endif()
		if(WIN32)
			message(STATUS "Using g++ (${_GXX}). Recommended generators: Ninja or MinGW Makefiles")
		endif()
		set(CMAKE_CXX_COMPILER "${_GXX}" CACHE FILEPATH "" FORCE)
	elseif(LIBCPPRIME_COMPILER STREQUAL "clang")
		find_program(_CLANGXX NAMES clang++)
		if(NOT _CLANGXX)
			message(FATAL_ERROR "clang++ not found in PATH")
		endif()
		if(WIN32)
			message(STATUS "Using clang++ (${_CLANGXX}). Recommended generator: Ninja")
		endif()
		set(CMAKE_CXX_COMPILER "${_CLANGXX}" CACHE FILEPATH "" FORCE)
	elseif(LIBCPPRIME_COMPILER STREQUAL "msvc")
		if(NOT WIN32)
			message(FATAL_ERROR "msvc is only supported on Windows")
		endif()
		if(NOT CMAKE_GENERATOR MATCHES "Visual Studio")
			find_program(_CL NAMES cl)
			if(_CL)
				set(CMAKE_CXX_COMPILER "${_CL}" CACHE FILEPATH "" FORCE)
			else()
				message(WARNING "MSVC 'cl' not found in PATH. Use a Visual Studio generator or Developer Command Prompt. Falling back to default.")
			endif()
		endif()
	endif()
endif()

project(libcpprime VERSION 0.0.0 LANGUAGES CXX)

# Options
option(LIBCPPRIME_BUILD_EXAMPLES "Build examples" ON)
option(LIBCPPRIME_BUILD_TESTS "Build tests" ON)
option(LIBCPPRIME_WARNINGS "Enable strict warnings for local targets" ON)
option(LIBCPPRIME_BUILD_BENCHMARKS "Build benchmarks" ON)

# Header-only library target
add_library(libcpprime INTERFACE)
target_include_directories(libcpprime INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)
target_compile_features(libcpprime INTERFACE cxx_std_20)

# Examples
if(LIBCPPRIME_BUILD_EXAMPLES)
	add_executable(example_basic examples/basic.cpp)
	target_link_libraries(example_basic PRIVATE libcpprime)

	if(LIBCPPRIME_WARNINGS)
		if(MSVC)
			target_compile_options(example_basic PRIVATE /W4 /permissive- /Zc:__cplusplus)
		else()
			target_compile_options(example_basic PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
		endif()
	endif()
endif()

# Tests
if(LIBCPPRIME_BUILD_TESTS)
	include(CTest)
	if(BUILD_TESTING)
		include(FetchContent)
		set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
		set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
		FetchContent_Declare(
			googletest
			GIT_REPOSITORY https://github.com/google/googletest.git
			GIT_TAG v1.14.0
		)
		FetchContent_MakeAvailable(googletest)

		add_executable(libcpprime_tests tests/test_isprime.cpp)
		target_link_libraries(libcpprime_tests PRIVATE libcpprime GTest::gtest_main)

		if(LIBCPPRIME_WARNINGS)
			if(MSVC)
				target_compile_options(libcpprime_tests PRIVATE
					$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:/Ox>
					/W4 /permissive- /Zc:__cplusplus
				)
			else()
				target_compile_options(libcpprime_tests PRIVATE -O3 -march=native -Wall -Wextra -Wpedantic -Wconversion)
			endif()
		endif()
		# Use generator expression to ensure correct config-specific path for multi-config generators
		add_test(NAME libcpprime_tests COMMAND $<TARGET_FILE:libcpprime_tests>)
		set_tests_properties(libcpprime_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
	endif()
endif()

# Benchmarks
if(LIBCPPRIME_BUILD_BENCHMARKS)
	include(FetchContent)
	# nanobench is a single-header library; we fetch and expose as an INTERFACE target
	FetchContent_Declare(
		nanobench
		GIT_REPOSITORY https://github.com/martinus/nanobench.git
		GIT_TAG v4.3.11
	)
	FetchContent_GetProperties(nanobench)
	if(NOT nanobench_POPULATED)
		FetchContent_Populate(nanobench)
		add_library(nanobench INTERFACE)
		target_include_directories(nanobench INTERFACE
			${nanobench_SOURCE_DIR}/src/include
			${nanobench_SOURCE_DIR}/include
		)
	endif()

	add_executable(benchmark_isprime benchmarks/isprime_bench.cpp)
	target_link_libraries(benchmark_isprime PRIVATE libcpprime nanobench)
	if(LIBCPPRIME_WARNINGS)
		if(MSVC)
			target_compile_options(benchmark_isprime PRIVATE /W4 /permissive- /Zc:__cplusplus)
		else()
			target_compile_options(benchmark_isprime PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
		endif()
	endif()
endif()

# Install (headers only)
include(GNUInstallDirs)
install(DIRECTORY libcpprime/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libcpprime)
install(TARGETS libcpprime EXPORT libcpprimeTargets)
install(EXPORT libcpprimeTargets
	NAMESPACE libcpprime::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libcpprime
)

# Usage examples (configure in a fresh build dir):
#   GCC:   cmake -S . -B build-gcc   -G "Ninja" -DLIBCPPRIME_COMPILER=gcc
#   Clang: cmake -S . -B build-clang -G "Ninja" -DLIBCPPRIME_COMPILER=clang
#   MSVC:  cmake -S . -B build-msvc  -G "Visual Studio 17 2022" -DLIBCPPRIME_COMPILER=msvc
# Build (multi-config VS): cmake --build build-msvc --config Release
# Run tests (multi-config VS): ctest --test-dir build-msvc -C Release
# Single-config generators:    ctest --test-dir build-gcc
