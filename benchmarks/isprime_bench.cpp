#include <nanobench.h>

#include <bit>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>
#include <limits>
#include <string>
#include <tuple>

int main() {
    using namespace ankerl::nanobench;

    auto start_time = std::chrono::high_resolution_clock::now();

    const char* out_prime = "benchmarks/bench_IsPrime.csv";
    const char* out_notable = "benchmarks/bench_IsPrimeNoTable.csv";
    const int samples = 32000;

    static std::uint32_t weighted[89440];
    for (std::uint32_t i = 1, count = 0; i <= 64; ++i) {
        for (std::uint32_t j = 0; j < i * i; ++j) {
            weighted[count++] = 64 - i;
        }
    }
    auto bench = [rng = Rng(42)](bool (*func)(std::uint64_t)) mutable {
        std::uint32_t k = weighted[rng.bounded(89440)];
        std::uint64_t n = (rng() >> k) | 1;
        constexpr std::uint32_t iters = 250;
        bool is_prime = func(n);
        auto min_time = std::numeric_limits<double>::max();
        for (std::uint32_t trial = 0; trial < 16; ++trial) {
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
        f << "n is_prime time_ns\n";
    }
    {
        std::ofstream f(out_notable, std::ios::binary);
        f << "n is_prime time_ns\n";
    }

    double time_prime_sum[65] = {};
    std::int32_t count_prime[65] = {};
    double time_composite_sum[65] = {};
    std::int32_t count_composite[65] = {};
    double time_prime_sum_NoTable[65] = {};
    std::int32_t count_prime_NoTable[65] = {};
    double time_composite_sum_NoTable[65] = {};
    std::int32_t count_composite_NoTable[65] = {};

    // Emit results for cppr::IsPrime
    {
        for (std::uint32_t i = 0; i < 4096; ++i) bench(&cppr::IsPrime);  // warmup
        std::ofstream f(out_prime, std::ios::binary | std::ios::app);
        f.setf(std::ios::fmtflags(0), std::ios::floatfield);  // default
        for (int i = 0; i < samples; ++i) {
            auto [n, isp, t] = bench(&cppr::IsPrime);
            char buf[96];
            int len = std::snprintf(buf, sizeof(buf), "%llu %d %.12f\n", static_cast<unsigned long long>(n), isp ? 1 : 0, t);
            f.write(buf, len);
            std::int32_t bitlen = std::bit_width(n);
            if (isp) {
                time_prime_sum[bitlen] += t;
                count_prime[bitlen] += 1;
            } else {
                time_composite_sum[bitlen] += t;
                count_composite[bitlen] += 1;
            }
        }
    }

    // Emit results for cppr::IsPrimeNoTable
    {
        for (std::uint32_t i = 0; i < 512; ++i) bench(&cppr::IsPrimeNoTable);  // warmup
        std::ofstream f(out_notable, std::ios::binary | std::ios::app);
        f.setf(std::ios::fmtflags(0), std::ios::floatfield);  // default
        for (int i = 0; i < samples; ++i) {
            auto [n, isp, t] = bench(&cppr::IsPrimeNoTable);
            char buf[96];
            int len = std::snprintf(buf, sizeof(buf), "%llu %d %.12f\n", static_cast<unsigned long long>(n), isp ? 1 : 0, t);
            f.write(buf, len);
            std::int32_t bitlen = std::bit_width(n);
            if (isp) {
                time_prime_sum_NoTable[bitlen] += t;
                count_prime_NoTable[bitlen] += 1;
            } else {
                time_composite_sum_NoTable[bitlen] += t;
                count_composite_NoTable[bitlen] += 1;
            }
        }
    }

    // Output summary
    if (std::filesystem::exists("benchmarks/bench_summary.md")) {
        std::filesystem::copy_file("benchmarks/bench_summary.md", "benchmarks/bench_summary_prev.md", std::filesystem::copy_options::overwrite_existing);
    }
    std::ofstream f_summary("benchmarks/bench_summary.csv", std::ios::trunc);
    std::ofstream f_summary_md("benchmarks/bench_summary.md", std::ios::trunc);
    f_summary << std::fixed << std::setprecision(6);
    f_summary << "avg_time_prime_IsPrime,avg_time_prime_IsPrimeNoTable,avg_time_composite_IsPrime,avg_time_composite_IsPrimeNoTable\n";
    f_summary_md << std::fixed << std::setprecision(3);
    f_summary_md << "| Bit Width | IsPrime Avg Time (ns) | IsPrimeNoTable Avg Time (ns) | IsPrime Avg Time (ns) | IsPrimeNoTable Avg Time (ns) |\n";
    f_summary_md << "|-----------|-----------------------|------------------------------|-----------------------|------------------------------|\n";
    for (std::int32_t i = 1; i <= 64; ++i) {
        double avg_prime = count_prime[i] ? (time_prime_sum[i] / count_prime[i]) : NAN;
        double avg_prime_NoTable = count_prime_NoTable[i] ? (time_prime_sum_NoTable[i] / count_prime_NoTable[i]) : NAN;
        double avg_composite = count_composite[i] ? (time_composite_sum[i] / count_composite[i]) : NAN;
        double avg_composite_NoTable = count_composite_NoTable[i] ? (time_composite_sum_NoTable[i] / count_composite_NoTable[i]) : NAN;
        f_summary << avg_prime << " " << avg_prime_NoTable << " " << avg_composite << " " << avg_composite_NoTable << "\n";
        f_summary_md << "| " << i << " | " << (std::isnan(avg_prime) ? "N/A" : std::to_string(avg_prime)) << " | " << (std::isnan(avg_prime_NoTable) ? "N/A" : std::to_string(avg_prime_NoTable))
                     << " | " << (std::isnan(avg_composite) ? "N/A" : std::to_string(avg_composite)) << " | " << (std::isnan(avg_composite_NoTable) ? "N/A" : std::to_string(avg_composite_NoTable))
                     << " |\n";
    }
    f_summary << std::flush;
    f_summary_md << std::flush;

    std::cout << "Benchmarking completed\n";
    std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count() << " seconds\n";

    return 0;
}
