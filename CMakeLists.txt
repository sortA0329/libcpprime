cmake_minimum_required(VERSION 3.20)

project(libcpprime VERSION 0.0.0 LANGUAGES CXX)

# Options
option(LIBCPPRIME_BUILD_EXAMPLES "Build examples" ON)
option(LIBCPPRIME_BUILD_TESTS "Build tests" ON)
option(LIBCPPRIME_WARNINGS "Enable strict warnings for local targets" ON)
option(LIBCPPRIME_BUILD_BENCHMARKS "Build benchmarks (ankerl::nanobench)" ON)

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
				target_compile_options(libcpprime_tests PRIVATE /Ox /W4 /permissive- /Zc:__cplusplus)
			else()
				target_compile_options(libcpprime_tests PRIVATE -O3 -march=native -Wall -Wextra -Wpedantic -Wconversion)
			endif()
		endif()

		# Ensure test data files are found; run tests with source dir as working directory
		add_test(NAME libcpprime_tests COMMAND libcpprime_tests)
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

