#include <chrono>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <limits>
#include <tuple>

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>

int main() {
    using namespace ankerl::nanobench;

    const char* out_prime = "benchmarks/bench_IsPrime.tsv";
    const char* out_notable = "benchmarks/bench_IsPrimeNoTable.tsv";
    const int samples = 32000;

    static std::uint32_t weighted[2080];
    for (std::uint32_t i = 1, count = 0; i <= 64; ++i) {
        for (std::uint32_t j = 0; j < i; ++j) {
            weighted[count++] = 64 - i;
        }
    }
    [[maybe_unused]] auto bench = [rng = Rng()](bool (*func)(std::uint64_t)) mutable {
        std::uint32_t k = weighted[rng.bounded(2080)];
        std::uint64_t n = (rng() >> k) | 1;
        constexpr std::uint32_t iters = 80;
        bool is_prime = func(n);
        auto min_time = std::numeric_limits<double>::max();
        for (std::uint32_t trial = 0; trial < 8; ++trial) {
            auto start = Clock::now();
            for (std::uint32_t i = 0; i != iters; ++i) {
                doNotOptimizeAway(func(n));
            }
            auto end = Clock::now();
            auto time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / iters;
            if (time < min_time) {
                min_time = time;
            }
        }
        return std::tuple{n, is_prime, min_time};
    };

    // Fast I/O settings
    std::ios::sync_with_stdio(false);

    // Write headers
    {
        std::ofstream f(out_prime, std::ios::binary);
        f << "n\tis_prime\ttime_ns\n";
    }
    {
        std::ofstream f(out_notable, std::ios::binary);
        f << "n\tis_prime\ttime_ns\n";
    }

    // Emit results for cppr::IsPrime
    {
        for (std::uint32_t i = 0; i < 4096; ++i) bench(&cppr::IsPrime);  // warmup
        std::ofstream f(out_prime, std::ios::binary | std::ios::app);
        f.setf(std::ios::fmtflags(0), std::ios::floatfield);  // default
        for (int i = 0; i < samples; ++i) {
            auto [n, isp, t] = bench(&cppr::IsPrime);
            char buf[96];
            int len = std::snprintf(buf, sizeof(buf), "%llu\t%d\t%.12f\n", static_cast<unsigned long long>(n), isp ? 1 : 0, t);
            f.write(buf, len);
        }
    }

    // Emit results for cppr::IsPrimeNoTable
    {
        for (std::uint32_t i = 0; i < 512; ++i) bench(&cppr::IsPrimeNoTable);  // warmup
        std::ofstream f(out_notable, std::ios::binary | std::ios::app);
        f.setf(std::ios::fmtflags(0), std::ios::floatfield);
        for (int i = 0; i < samples; ++i) {
            auto [n, isp, t] = bench(&cppr::IsPrimeNoTable);
            char buf[96];
            int len = std::snprintf(buf, sizeof(buf), "%llu\t%d\t%.12f\n", static_cast<unsigned long long>(n), isp ? 1 : 0, t);
            f.write(buf, len);
        }
    }

    std::cout << "Benchmarking completed.\n";

    return 0;
}
