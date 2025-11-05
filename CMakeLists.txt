cmake_minimum_required(VERSION 3.20)
project(libcpprime LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags
if(MSVC)
    # MSVC (including MinGW with MSVC-compatible clang++)
    add_compile_options(/W4 /permissive-)
else()
    # GCC, Clang
    add_compile_options(-Wall -Wextra -Wpedantic -Werror=implicit-function-declaration)
    if(APPLE)
        add_compile_options(-fPIC)
    endif()
endif()

# Set optimization flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        add_compile_options(/O2)
    else()
        add_compile_options(-O3)
    endif()
endif()

# Enable clangd support by generating compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Header-only library
add_library(libcpprime INTERFACE)
target_include_directories(libcpprime INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Enable testing
enable_testing()

# Google Test integration
include(FetchContent)

# Set policy for FetchContent timestamp
cmake_policy(SET CMP0135 NEW)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)

# For Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Tests
add_executable(libcpprime_tests tests/test_isprime.cpp)
target_link_libraries(libcpprime_tests PRIVATE libcpprime gtest gtest_main)
target_include_directories(libcpprime_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME LibcpprimeTests COMMAND libcpprime_tests)
