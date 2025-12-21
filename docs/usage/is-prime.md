# `<libcpprime/IsPrime.hpp>`

## `cppr::IsPrime()`

```cpp
namespace cppr {
    bool IsPrime(std::uint64_t n) noexcept; // C++11
    constexpr bool IsPrime(std::uint64_t n) noexcept; // C++20
}
```

It returns true if the input value is a prime number; otherwise, it returns false.

## example

```cpp
#include <libcpprime/IsPrime.hpp>
#include <cassert>
int main() {
    assert(cppr::IsPrime(998244353) == true);
    assert(cppr::IsPrime(13148563482635885461) == false);
}
```
