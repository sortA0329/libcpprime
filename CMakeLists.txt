cmake_minimum_required(VERSION 3.20)

# Compiler selection (default/gcc/clang/clang-cl/msvc). Effective only at first configure.
set(LIBCPPRIME_COMPILER "default" CACHE STRING "Select C++ compiler: default/gcc/clang/clang-cl/msvc")
set_property(CACHE LIBCPPRIME_COMPILER PROPERTY STRINGS default gcc clang clang-cl msvc)

# Generator compatibility check (gcc/clang cannot be used with Visual Studio generator)
if(LIBCPPRIME_COMPILER MATCHES "^(gcc|clang)$" AND CMAKE_GENERATOR MATCHES "Visual Studio")
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
	elseif(LIBCPPRIME_COMPILER STREQUAL "clang-cl")
		if(NOT WIN32)
			message(FATAL_ERROR "clang-cl is only supported on Windows")
		endif()
		find_program(_CLANGCL NAMES clang-cl)
		if(NOT _CLANGCL)
			message(FATAL_ERROR "clang-cl not found in PATH")
		endif()
		# For Visual Studio generators, prefer selecting the ClangCL toolset.
		if(CMAKE_GENERATOR MATCHES "Visual Studio")
			set(CMAKE_GENERATOR_TOOLSET "ClangCL" CACHE STRING "" FORCE)
			message(STATUS "Using clang-cl via Visual Studio toolset ClangCL")
		else()
			message(STATUS "Using clang-cl (${_CLANGCL}). Recommended generator: Ninja")
			set(CMAKE_C_COMPILER "${_CLANGCL}" CACHE FILEPATH "" FORCE)
			set(CMAKE_CXX_COMPILER "${_CLANGCL}" CACHE FILEPATH "" FORCE)
		endif()
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

# clang-cl is Clang, but MSVC-like.
set(LIBCPPRIME_CLANG_CL FALSE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
	set(LIBCPPRIME_CLANG_CL TRUE)
endif()

set(LIBCPPRIME_MSVC_LIKE FALSE)
if(MSVC OR LIBCPPRIME_CLANG_CL)
	set(LIBCPPRIME_MSVC_LIKE TRUE)
endif()

# Options
option(LIBCPPRIME_BUILD_EXAMPLES "Build examples" ON)
option(LIBCPPRIME_BUILD_TESTS "Build tests" ON)
option(LIBCPPRIME_WARNINGS "Enable strict warnings for local targets" ON)
option(LIBCPPRIME_BUILD_BENCHMARKS "Build benchmarks" ON)

# Header-only library target
add_library(libcpprime INTERFACE)
target_include_directories(libcpprime INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
target_compile_features(libcpprime INTERFACE cxx_std_20)

# Examples
if(LIBCPPRIME_BUILD_EXAMPLES)
	add_executable(example_basic examples/basic.cpp)
	target_link_libraries(example_basic PRIVATE libcpprime)

	if(LIBCPPRIME_WARNINGS)
		if(LIBCPPRIME_MSVC_LIKE)
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
		add_executable(libcpprime_tests tests/test_isprime.cpp)

		include(FetchContent)
		set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
		set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
		FetchContent_Declare(
			googletest
			GIT_REPOSITORY https://github.com/google/googletest.git
			GIT_TAG v1.17.0
		)
		set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
		FetchContent_MakeAvailable(googletest)
		target_link_libraries(libcpprime_tests PRIVATE libcpprime GTest::gtest_main)

		FetchContent_Declare(
    		primesieve
    		GIT_REPOSITORY https://github.com/kimwalisch/primesieve.git
    		GIT_TAG v12.10
		)
		FetchContent_MakeAvailable(primesieve)
		target_link_libraries(libcpprime_tests PRIVATE primesieve::primesieve)

		if(LIBCPPRIME_WARNINGS)
			if(LIBCPPRIME_CLANG_CL)
				target_compile_options(libcpprime_tests PRIVATE
					$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:/Ox>
					/W4 /permissive- /Zc:__cplusplus
					/clang:-fconstexpr-steps=100000000 /clang:-fconstexpr-depth=2048
				)
			elseif(MSVC)
				target_compile_options(libcpprime_tests PRIVATE
					$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:/Ox>
					/W4 /permissive- /Zc:__cplusplus
					/constexpr:steps10000000 /constexpr:depth2048
				)
			else()
				target_compile_options(libcpprime_tests PRIVATE
				    -O3 -march=native -Wall -Wextra -Wpedantic -Wconversion
				)
				if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
					target_compile_options(libcpprime_tests PRIVATE -fconstexpr-steps=100000000 -fconstexpr-depth=2048)
				endif()
			endif()
		endif()
		# Use generator expression to ensure correct config-specific path for multi-config generators
		add_test(NAME libcpprime_tests COMMAND $<TARGET_FILE:libcpprime_tests>)
	endif()
endif()

# Benchmarks
if(LIBCPPRIME_BUILD_BENCHMARKS)
	add_executable(benchmark_isprime benchmarks/isprime_bench.cpp)

	include(FetchContent)
	FetchContent_Declare(
		nanobench
		GIT_REPOSITORY https://github.com/martinus/nanobench.git
		GIT_TAG v4.3.11
	)
	FetchContent_MakeAvailable(nanobench)
	target_link_libraries(benchmark_isprime PRIVATE libcpprime nanobench::nanobench)

	if(LIBCPPRIME_WARNINGS)
		if(LIBCPPRIME_MSVC_LIKE)
			target_compile_options(benchmark_isprime PRIVATE /W4 /permissive- /Zc:__cplusplus)
		else()
			target_compile_options(benchmark_isprime PRIVATE -O3 -Wall -Wextra -Wpedantic -Wconversion)
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
