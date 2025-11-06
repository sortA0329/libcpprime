#include <chrono>
#include <cstdint>
#include <tuple>

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>

int main() {
    using namespace ankerl::nanobench;

    static std::uint32_t weighted[2080];
    for (std::uint32_t i = 1, count = 0; i <= 64; ++i) {
        for (std::uint32_t j = 0; j < i; ++j) {
            weighted[count++] = 64 - i;
        }
    }
    [[maybe_unused]] auto bench = [rng = Rng()](bool (*func)(std::uint64_t)) mutable {
        std::uint32_t k = weighted[rng.bounded(2080)];
        std::uint64_t n = (rng() >> k) | 1;
        std::uint32_t iters = 8 * (k + 8);
        bool is_prime = func(n);  // warmup
        auto start = Clock::now();
        for (std::uint32_t i = 0; i != iters; ++i) {
            doNotOptimizeAway(func(n));
        }
        auto end = Clock::now();
        auto time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / iters;
        return std::tuple{n, is_prime, time};
    };

    return 0;
}
