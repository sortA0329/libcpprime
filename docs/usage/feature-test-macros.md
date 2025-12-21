# `<libcpprime/FeatureTestMacros.hpp>`

## `CPPR_HAS_CONSTEXPR_IS_PRIME`

```cpp
#define CPPR_HAS_CONSTEXPR_IS_PRIME 1 // C++20
```

This is a feature test macro that determines whether `cppr::IsPrime` and `cppr::IsPrimeNoTable` are declared with `constexpr`.

## example

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
