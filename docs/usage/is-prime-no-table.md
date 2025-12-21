# `<libcpprime/IsPrimeNoTable.hpp>`

## `cppr::IsPrimeNoTable()`

```cpp
namespace cppr {
    bool IsPrimeNoTable(std::uint64_t n) noexcept; // C++11
    constexpr bool IsPrimeNoTable(std::uint64_t n) noexcept; // C++20
}
```

It returns true if the input value is a prime number; otherwise, it returns false.
If you want to reduce the size of the executable file, use this function instead of `cppr::IsPrime` because `cppr::IsPrime` uses a 36KB table for performance optimization.

## example

```cpp
#include <libcpprime/IsPrimeNoTable.hpp>
#include <cassert>
int main() {
    assert(cppr::IsPrimeNoTable(998244353) == true);
    assert(cppr::IsPrimeNoTable(1314856348263588546) == false);
}
```
