cmake_minimum_required(VERSION 3.20)

# ================================
# Project
# ================================
project(libcpprime VERSION 0.0.0 LANGUAGES CXX)

# ================================
# Build type handling
# ================================
# Single-config generators (Ninja, Makefiles): default to Release
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ================================
# Compiler selection
# ================================
set(LIBCPPRIME_COMPILER "default" CACHE STRING "Select C++ compiler: default/gcc/clang/clang-cl/msvc")
set_property(CACHE LIBCPPRIME_COMPILER PROPERTY STRINGS default gcc clang clang-cl msvc)

# gcc / clang cannot be used with Visual Studio generator
if(LIBCPPRIME_COMPILER MATCHES "^(gcc|clang)$" AND CMAKE_GENERATOR MATCHES "Visual Studio")
	message(FATAL_ERROR
		"gcc/clang cannot be used with Visual Studio generator.\n"
		"Use Ninja or Makefiles instead."
	)
endif()

if(NOT DEFINED CMAKE_CXX_COMPILER)
	if(LIBCPPRIME_COMPILER STREQUAL "gcc")
		find_program(_GXX g++)
		if(NOT _GXX)
			message(FATAL_ERROR "g++ not found")
		endif()
		set(CMAKE_CXX_COMPILER "${_GXX}" CACHE FILEPATH "" FORCE)

	elseif(LIBCPPRIME_COMPILER STREQUAL "clang")
		find_program(_CLANGXX clang++)
		if(NOT _CLANGXX)
			message(FATAL_ERROR "clang++ not found")
		endif()
		set(CMAKE_CXX_COMPILER "${_CLANGXX}" CACHE FILEPATH "" FORCE)

	elseif(LIBCPPRIME_COMPILER STREQUAL "clang-cl")
		if(NOT WIN32)
			message(FATAL_ERROR "clang-cl is Windows only")
		endif()
		find_program(_CLANGCL clang-cl)
		if(NOT _CLANGCL)
			message(FATAL_ERROR "clang-cl not found")
		endif()

		if(CMAKE_GENERATOR MATCHES "Visual Studio")
			set(CMAKE_GENERATOR_TOOLSET "ClangCL" CACHE STRING "" FORCE)
		else()
			set(CMAKE_C_COMPILER "${_CLANGCL}" CACHE FILEPATH "" FORCE)
			set(CMAKE_CXX_COMPILER "${_CLANGCL}" CACHE FILEPATH "" FORCE)
		endif()

	elseif(LIBCPPRIME_COMPILER STREQUAL "msvc")
		if(NOT WIN32)
			message(FATAL_ERROR "msvc is Windows only")
		endif()
	endif()
endif()

# ================================
# Compiler kind detection
# ================================
set(LIBCPPRIME_CLANG_CL FALSE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
	AND CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
	set(LIBCPPRIME_CLANG_CL TRUE)
endif()

set(LIBCPPRIME_MSVC_LIKE FALSE)
if(MSVC OR LIBCPPRIME_CLANG_CL)
	set(LIBCPPRIME_MSVC_LIKE TRUE)
endif()

# ================================
# MSVC / clang-cl runtime
# ================================
if(LIBCPPRIME_MSVC_LIKE)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
	set(CMAKE_MSVC_RUNTIME_LIBRARY_DEBUG "MultiThreadedDLLDebug")
endif()

# ================================
# Options
# ================================
option(LIBCPPRIME_BUILD_EXAMPLES   "Build examples"   ON)
option(LIBCPPRIME_BUILD_TESTS      "Build tests"      ON)
option(LIBCPPRIME_BUILD_BENCHMARKS "Build benchmarks" ON)

# ================================
# Header-only library
# ================================
add_library(libcpprime INTERFACE)
target_include_directories(libcpprime INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

# ================================
# Optimization flags
# ================================
function(libcpprime_apply_optimization target)
	if(LIBCPPRIME_MSVC_LIKE)
		target_compile_options(${target} PRIVATE
			/Ox /arch:AVX2
		)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
		target_compile_options(${target} PRIVATE
			-O3 -march=native
		)
	endif()
endfunction()

# ================================
# Examples
# ================================
if(LIBCPPRIME_BUILD_EXAMPLES)
	add_executable(example_basic examples/basic.cpp)
	target_link_libraries(example_basic PRIVATE libcpprime)
	target_compile_features(example_basic PRIVATE cxx_std_23)

	if(LIBCPPRIME_MSVC_LIKE)
		target_compile_options(example_basic PRIVATE /W4 /permissive- /Zc:__cplusplus)
	else()
		target_compile_options(example_basic PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
	endif()
endif()

# ================================
# Tests
# ================================
if(LIBCPPRIME_BUILD_TESTS)
	include(CTest)
	if(BUILD_TESTING)
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

		function(libcpprime_apply_test_options target)
			if(LIBCPPRIME_CLANG_CL)
				target_compile_options(${target} PRIVATE
					/W4 /permissive- /Zc:__cplusplus
					/clang:-fconstexpr-steps=10000000
				)
			elseif(MSVC)
				target_compile_options(${target} PRIVATE
					/W4 /permissive- /Zc:__cplusplus
					/constexpr:steps10000000
				)
			elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
				target_compile_options(${target} PRIVATE
					-Wall -Wextra -Wpedantic -Wconversion
					-fconstexpr-steps=10000000
				)
			else()
				target_compile_options(${target} PRIVATE
				-Wall -Wextra -Wpedantic -Wconversion
				)
			endif()
			libcpprime_apply_optimization(${target})
		endfunction()

		function(libcpprime_add_isprime_test target cxx_standard)
			add_executable(${target} tests/isprime_tests.cpp)
			target_link_libraries(${target} PRIVATE libcpprime GTest::gtest_main)

			set_property(TARGET ${target} PROPERTY CXX_STANDARD ${cxx_standard})
			set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)
			set_property(TARGET ${target} PROPERTY CXX_EXTENSIONS OFF)

			libcpprime_apply_test_options(${target})
			add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
		endfunction()

		set(_libcpprime_isprime_standards 23 20 17 14 11)
		set(_libcpprime_isprime_targets "")
		foreach(_std IN LISTS _libcpprime_isprime_standards)
			set(_tgt "test_isprime_cxx${_std}")
			libcpprime_add_isprime_test(${_tgt} ${_std})
			list(APPEND _libcpprime_isprime_targets ${_tgt})
		endforeach()

		add_custom_target(test_isprime DEPENDS ${_libcpprime_isprime_targets})
	endif()
endif()

# ================================
# Benchmarks
# ================================
if(LIBCPPRIME_BUILD_BENCHMARKS)
	add_executable(benchmark_isprime benchmarks/isprime_bench.cpp)
	target_compile_features(benchmark_isprime PRIVATE cxx_std_23)

	target_link_libraries(benchmark_isprime PRIVATE libcpprime)

	if(LIBCPPRIME_MSVC_LIKE)
		target_compile_options(benchmark_isprime PRIVATE /W4 /permissive- /Zc:__cplusplus)
	else()
		target_compile_options(benchmark_isprime PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
	endif()

	libcpprime_apply_optimization(benchmark_isprime)
endif()

# ================================
# Install
# ================================
include(GNUInstallDirs)
install(DIRECTORY libcpprime/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libcpprime)
install(TARGETS libcpprime EXPORT libcpprimeTargets)
install(EXPORT libcpprimeTargets
	NAMESPACE libcpprime::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libcpprime
)
