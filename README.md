# libcpprime

![badge](https://github.com/sortA0329/libcpprime/actions/workflows/tests.yml/badge.svg)

**libcpprime** is an efficient C++ implementation of a primality test optimized for 64-bit integers.

## Usage

### `cppr::IsPrime()`

Header: `<libcpprime/IsPrime.hpp>`

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
    assert(cppr::IsPrime(13148563482635885461) == false);
}
```

### `cppr::IsPrimeCompact`

Header: `<libcpprime/IsPrimeCompact.hpp>`

```cpp
namespace cppr {
    bool IsPrimeCompact(std::uint64_t n) noexcept; // C++11
    constexpr bool IsPrimeCompact(std::uint64_t n) noexcept; // C++20
}
```

It returns true if the input value is a prime number; otherwise, it returns false.
If you want to reduce the size of the executable file, use this function instead of `cppr::IsPrime` because `cppr::IsPrime` uses a 640KB table for performance optimization.

#### example

```cpp
#include <libcpprime/IsPrimeCompact.hpp>
#include <cassert>
int main() {
    assert(cppr::IsPrimeCompact(998244353) == true);
    assert(cppr::IsPrimeCompact(1314856348263588546) == false);
}
```

### `CPPR_HAS_CONSTEXPR_IS_PRIME`

Header: `<libcpprime/FeatureTestMacros.hpp>`

```cpp
#define CPPR_HAS_CONSTEXPR_IS_PRIME 1 // C++20
```

This is a feature test macro that determines whether `cppr::IsPrime` and `cppr::IsPrimeCompact` are declared with `constexpr`.

#### example

```cpp
#include <libcpprime/FeatureTestMacros.hpp>
#include <libcpprime/IsPrime.hpp>
#include <iostream>
int main() {
#ifdef CPPR_HAS_CONSTEXPR_IS_PRIME
    constexpr bool x = cppr::IsPrime(1000000007);
#else
    const bool x = cppr::IsPrime(1000000007);
#endif
    std::cout << x << std::endl;
}
```

## Requirements

- C++11
- GCC, Clang, GCC (MinGW), Clang (MinGW), MSVC, clang-cl

## Compilation

This library is header-only, so you only need to specify the include path.

```
g++ -I ./libcpprime -O3 Main.cpp
```

## Benchmarks

Benchmarks are executed on GitHub Actions.

Workflow: [bench.yml](https://github.com/sortA0329/libcpprime/actions/workflows/bench.yml)

### Linux (gcc)

[View Summary](https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Linux-gcc/bench_summary.md)

<p>
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Linux-gcc/bench_summary.webp"
        width="500px"
        alt="Linux gcc summary"
    />
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Linux-gcc/bench_IsPrime.webp"
        width="500px"
        alt="Linux gcc IsPrime"
    />
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Linux-gcc/bench_IsPrimeCompact.webp"
        width="500px"
        alt="Linux gcc IsPrimeCompact"
    />
</p>

### Linux (clang)

[View summary](https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Linux-clang/bench_summary.md)

<p>
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Linux-clang/bench_summary.webp"
        width="500px"
        alt="Linux clang summary"
    />
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Linux-clang/bench_IsPrime.webp"
        width="500px"
        alt="Linux clang IsPrime"
    />
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Linux-clang/bench_IsPrimeCompact.webp"
        width="500px"
        alt="Linux clang IsPrimeCompact"
    />
</p>

### Windows (msvc)

[View Summary](https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Windows-msvc/bench_summary.md)

<p>
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Windows-msvc/bench_summary.webp"
        width="500px"
        alt="Windows msvc summary"
    />
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Windows-msvc/bench_IsPrime.webp"
        width="500px"
        alt="Windows msvc IsPrime"
    />
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Windows-msvc/bench_IsPrimeCompact.webp"
        width="500px"
        alt="Windows msvc IsPrimeCompact"
    />
</p>

### Windows (clang-cl)

[View Summary](https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Windows-clang-cl/bench_summary.md)

<p>
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Windows-clang-cl/bench_summary.webp"
        width="500px"
        alt="Windows clang-cl summary"
    />
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Windows-clang-cl/bench_IsPrime.webp"
        width="500px"
        alt="Windows clang-cl IsPrime"
    />
    <img
        src="https://sortA0329.github.io/libcpprime/benchmarks/latest/benchmark-Windows-clang-cl/bench_IsPrimeCompact.webp"
        width="500px"
        alt="Windows clang-cl IsPrimeCompact"
    />
</p>

## Algorithm

The core algorithm of `libcpprime` is the Miller–Rabin primality test. Although traditionally a randomized algorithm, it has been proven that any integer below 2^64 can be deterministically tested using seven fixed bases [1].

Using a hash table optimizes this process further. For composite numbers that act as pseudoprimes to a specific base (such as base 2), their hash values are calculated to assign them into separate buckets. Bases are then selected for each bucket to correctly test every number mapped to it. Finding these optimal bases took under a minute, aided by GPU acceleration and precomputed base-2 pseudoprimes. [2]

Thanks to this strategy, `cppr::IsPrime` requires only 2 bases, while `cppr::IsPrimeCompact` uses 5. For numbers smaller than 2^32, Bradley Berg's algorithm is applied, which relies on a single base [3].

Beyond core algorithmic choices, micro-optimizations play a critical role. Because CPU division takes longer than multiplication, Montgomery reduction is employed. For numbers below 2^32, accepting both `MR(x)` and `MR(x) + n` (where `MR` denotes the Montgomery representation) eliminates unpredictable conditional branches. For values smaller than 2^21, fast modulo reduction using Lemire's method is applied [4] [5].

Furthermore, tests against multiple bases are executed concurrently within a single loop. This design maximizes instruction-level parallelism (ILP) and minimizes conditional branching. Although this increases execution time for composite numbers that would otherwise be filtered out early in sequential testing, `libcpprime` prioritizes optimizing worst-case performance.

Additional micro-optimizations include:

- Trial division by small primes
- Precomputed lookup flags for small numbers below 2^21 (`cppr::IsPrime`) or 2^10 (`cppr::IsPrimeCompact`)
- Skipping the second step of the Miller–Rabin test (repeated squaring) when n ≡ 3 mod 4
- Newton's method for computing `-n^-1 mod R` [6]
- GCD calculation with products of small primes using Stein's algorithm (binary GCD) instead of division [7]
- Standard division for numbers between 2^21 and 2^32
- Inline assembly (GCC/Clang) and `_udiv128` (MSVC) for 128-bit modulo arithmetic, avoiding overhead from compiler built-in functions for `unsigned __int128`
- `libdivide` for 128-bit modulo arithmetic when hardware/intrinsic support is unavailable
- Compiler hints (e.g., `__builtin_assume`) providing value ranges for improved code generation

[1] https://miller-rabin.appspot.com/  
[2] https://www.cecm.sfu.ca/Pseudoprimes/index-2-to-64.html  
[3] https://www.techneon.com/download/is.prime.32.base.data  
[4] https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/  
[5] https://en.algorithmica.org/hpc/arithmetic/division/  
[6] https://rsk0315.hatenablog.com/entry/2022/11/27/060616  
[7] https://lpha-z.hatenablog.com/entry/2020/05/31/231500

## Releases

- 2026/10/04 v1.4.1
  - Improve performance of `cppr::IsPrime`, which resulted in a 600KB increase in table size
- 2026/10/01 v1.4.0
  - Rename `cppr::IsPrimeNoTable` to `cppr::IsPrimeCompact`
  - Improve performance of `cppr::IsPrimeCompact`
- 2026/09/25 v1.3.5
  - Improve performance of `cppr::IsPrimeNoTable`
- 2026/06/28 v1.3.4
  - Improve performance
  - Prevent unnecessary code from being included when `FeatureTestMacros.hpp` is included
  - Fix the name and link in the copyright notice
  - Add the copyright notice for Bradley Berg's algorithm to the LICENSE file
- 2026/02/25 v1.3.3
  - Update copyright year to 2026
  - Add -O3 -march=native to the compilation flags during benchmarking and testing
- 2026/01/04 v1.3.2
  - Improve performance
- 2025/12/24 v1.3.1
  - Improve performance and reduce binary size for `cppr::IsPrime`
- 2025/12/21 v1.3.0
  - Add `CPPR_HAS_CONSTEXPR_IS_PRIME`
  - Support clang-cl
  - Accelerating Compile-Time Computation
  - Improved compatibility
- 2025/03/10 v1.2.11
  - Change the name on the license
  - Change Multiprication Algorithm
  - Replace `__uint128_t` with `unsigned __int128`
- 2025/01/05 v1.2.10
  - Change the condition of `constexpr`
- 2025/01/03 v1.2.9
  - Fix a bug
- 2025/01/02 v1.2.8
  - Improve performance
  - Suppress warnings
- 2024/12/31 v1.2.7
  - Improve performance
- 2024/12/30 v1.2.6
  - Improve performance
- 2024/12/29 v1.2.5
  - Add copyrights notice
- 2024/12/28 v1.2.4
  - Improve performance
- 2024/12/26 v1.2.3
  - Improve performance
- 2024/12/25 v1.2.2
  - Improve performance
- 2024/12/23 v1.2.1
  - Improve performance
- 2024/12/19 v1.2.0
  - Split `cppr::IsPrime` into `cppr::IsPrime` and `cppr::IsPrimeNoTable`
- 2024/12/19 v1.1.2
  - Fix typo
- 2024/12/18 v1.1.1
  - Add include guards
- 2024/12/18 v1.1.0
  - Add `cppr::IsPrime` with a table
- 2024/12/18 v1.0.0
  - Add `cppr::IsPrime`
