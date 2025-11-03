# libcpprime

**libcpprime** is a efficient C++ implementation of a primality test optimized for 64-bit integers.

# Usage

## <libcpprime/IsPrime.hpp>

### `cppr::IsPrime()`

```cpp
namespace cppr {
    bool IsPrime(std::uint64_t n) noexcept; // C++11
    constexpr bool IsPrime(std::uint64_t n) noexcept; // C++20
}
```

It returns true if the input value is a prime number; otherwise, it returns false.

#### example

```cpp
#include <libcpprime/IsPrime.hpp>
#include <cassert>
int main() {
    assert(cppr::IsPrime(998244353) == true);
    assert(cppr::IsPrime(999988224444335533) == false);
}
```

## <libcpprime/IsPrimeNoTable.hpp>

### `cppr::IsPrimeNoTable`

```cpp
namespace cppr {
    bool IsPrimeNoTable(std::uint64_t n) noexcept; // C++11
    constexpr bool IsPrimeNoTable(std::uint64_t n) noexcept; // C++20
}
```

It returns true if the input value is a prime number; otherwise, it returns false.
If you want to reduce the size of the executable file, use this function instead of `cppr::IsPrime` because `cppr::IsPrime` uses a 36KB table for performance optimization.

#### example

```cpp
#include <libcpprime/IsPrimeNoTable.hpp>
#include <cassert>
int main() {
    assert(cppr::IsPrimeNoTable(998244353) == true);
    assert(cppr::IsPrimeNoTable(999988224444335533) == false);
}
```

# Requirements

-   C++11
-   Any compiler that supports C++11

# Compilation

This library is header-only, so you only need to specify the include path.

```
g++ -I ./libcpprime -O3 Main.cpp
```

## Building with CMake

libcpprime uses CMake for building and testing. The project supports multiple platforms and compilers.

### Prerequisites

- CMake 3.20 or later
- A C++20 compatible compiler (clang++, g++, MSVC, or MinGW)
- Ninja (recommended) or other CMake generators

### Quick Start

```bash
# Configure (Release build with clang++)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run tests
ctest --build-config Release --output-on-failure --verbose
```

### For Different Configurations

#### Debug Build
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --build-config Debug --output-on-failure
```

#### Using GCC instead of Clang
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++
cmake --build build
```

#### Using MSVC on Windows
```bash
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

#### Using MinGW on Windows
```bash
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++
cmake --build build
```

### Running Tests

The test suite includes:
- Basic tests with known prime and composite numbers
- Data-driven tests using test data files (Primes.txt, Composites.txt)
- Constexpr compile-time tests (C++20 only)

```bash
# Run all tests
ctest --build-config Release --output-on-failure --verbose

# Run specific test
ctest --build-config Release -R "IsPrimeBasicTest" --output-on-failure
```

### VS Code Integration

The project includes VS Code configuration for:
- **clangd** support with compile_commands.json
- CMake build and test tasks
- Debug configurations
- C++ IntelliSense

#### Recommended Extensions
- clangd (llvm-vs-code-extensions.vscode-clangd)
- CMake Tools (ms-vscode.cmake-tools)
- C/C++ (ms-vscode.cpptools)

#### Quick Tasks in VS Code
- `Ctrl+Shift+B`: Build (default task)
- `Ctrl+Shift+T`: Run tests
- `F5`: Debug tests or examples

# Performance

### `cppr::IsPrime`

<img src="./README/IsPrime.png" width="400">

### `cppr::IsPrimeNoTable`

<img src="./README/IsPrimeNoTable.png" width="400">

# Releases

-   2025/03/10 ver 1.2.11
    -   Change the name on the license
    -   Change Multiprication Algorithm
    -   Replace `__uint128_t` with `unsigned __int128`
-   2025/01/05 ver 1.2.10
    -   Change the condition of `constexpr`
-   2025/01/03 ver 1.2.9
    -   Fix a bug
-   2025/01/02 ver 1.2.8
    -   Improve performance
    -   Suppress warnings
-   2024/12/31 ver 1.2.7
    -   Improve performance
-   2024/12/30 ver 1.2.6
    -   Improve performance
-   2024/12/29 ver 1.2.5
    -   Add copyrights notice
-   2024/12/28 ver 1.2.4
    -   Improve performance
-   2024/12/26 ver 1.2.3
    -   Improve performance
-   2024/12/25 ver 1.2.2
    -   Improve performance
-   2024/12/23 ver 1.2.1
    -   Improve performance
-   2024/12/19 ver 1.2.0
    -   Split `cppr::IsPrime` into `cppr::IsPrime` and `cppr::IsPrimeNoTable`
-   2024/12/19 ver 1.1.2
    -   Fix typo
-   2024/12/18 ver 1.1.1
    -   Add include guards
-   2024/12/18 ver 1.1.0
    -   Add `cppr::IsPrime` with a table
-   2024/12/18 ver 1.0.0
    -   Add `cppr::IsPrime`
