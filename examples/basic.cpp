#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>
#include <iostream>
#include <cstdint>

int main() {
    // Example usage of libcpprime
    std::cout << "libcpprime - C++ Primality Test Library" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // Test some known primes and composites
    std::uint64_t test_cases[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 998244353, 1000000007, 999988224444335533ULL };

    std::cout << "Testing cppr::IsPrime():" << std::endl;
    for (const auto& n : test_cases) {
        std::cout << n << ": " << (cppr::IsPrime(n) ? "Prime" : "Composite") << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Testing cppr::IsPrimeNoTable():" << std::endl;
    for (const auto& n : test_cases) {
        std::cout << n << ": " << (cppr::IsPrimeNoTable(n) ? "Prime" : "Composite") << std::endl;
    }

    return 0;
}
