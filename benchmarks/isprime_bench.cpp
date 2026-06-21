#include <bit>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <utility>

#if defined(_MSC_VER)

template <typename T>
void doNotOptimizeAway(T const& val);

#else

// see https://github.com/google/benchmark/blob/v1.7.1/include/benchmark/benchmark.h#L443-L446
template <typename T>
void doNotOptimizeAway(T const& val) {
    asm volatile("" : : "r,m"(val) : "memory");
}

template <typename T>
void doNotOptimizeAway(T& val) {
#if defined(__clang__)
    asm volatile("" : "+r,m"(val) : : "memory");
#else
    asm volatile("" : "+m,r"(val) : : "memory");
#endif
}
#endif

int main(int argc, char** argv) {
    bool heavy = (argc > 1 && std::string(argv[1]) == "--heavy");

    auto start_time = std::chrono::high_resolution_clock::now();

    const int samples = heavy ? 64000 : 32000;

    static std::uint32_t weighted[89440];
    for (std::uint32_t i = 1, count = 0; i <= 64; ++i) {
        for (std::uint32_t j = 0; j < i * i; ++j) {
            weighted[count++] = 64 - i;
        }
    }

    auto bench = [heavy](std::mt19937_64& rng, std::uniform_int_distribution<>& uniform, bool (*func)(std::uint64_t)) mutable {
        std::uint32_t k = weighted[uniform(rng)];
        std::uint64_t n = (rng() >> k) | 1;
        int iters = (heavy ? 300 : 100);
        bool is_prime = func(n);
        auto min_time = std::numeric_limits<double>::max();
        for (int trial = 0; trial < (heavy ? 24 : 16); ++trial) {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < iters; ++i) {
                doNotOptimizeAway(func(n));
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto time = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / iters;
            if (time < min_time) {
                min_time = time;
            }
        }
        return std::tuple{n, is_prime, min_time};
    };

    // Fast I/O settings
    std::ios::sync_with_stdio(false);

    double time_prime_sum[65] = {};
    std::int32_t count_prime[65] = {};
    double time_composite_sum[65] = {};
    std::int32_t count_composite[65] = {};
    double time_prime_sum_NoTable[65] = {};
    std::int32_t count_prime_NoTable[65] = {};
    double time_composite_sum_NoTable[65] = {};
    std::int32_t count_composite_NoTable[65] = {};

    // Emit results for cppr::IsPrime
    auto bench_IsPrime = [&] {
        std::mt19937_64 rng(100);
        std::uniform_int_distribution<> uniform(0, 89439);
        for (std::uint32_t i = 0; i < 4096; ++i) bench(rng, uniform, &cppr::IsPrime);  // warmup
        std::ofstream f("benchmarks/bench_IsPrime.csv", std::ios::trunc);
        f << "n,is_prime,time_ns\n";
        f.setf(std::ios::fmtflags(0), std::ios::floatfield);  // default
        for (int i = 0; i < samples; ++i) {
            auto [n, isp, t] = bench(rng, uniform, &cppr::IsPrime);
            char buf[96];
            int len = std::snprintf(buf, sizeof(buf), "%llu,%d,%.12f\n", static_cast<unsigned long long>(n), isp ? 1 : 0, t);
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
    };

    // Emit results for cppr::IsPrimeNoTable
    auto bench_IsPrimeNoTable = [&] {
        std::mt19937_64 rng(100);
        std::uniform_int_distribution<> uniform(0, 89439);
        for (std::uint32_t i = 0; i < 512; ++i) bench(rng, uniform, &cppr::IsPrimeNoTable);  // warmup
        std::ofstream f("benchmarks/bench_IsPrimeNoTable.csv", std::ios::trunc);
        f << "n,is_prime,time_ns\n";
        f.setf(std::ios::fmtflags(0), std::ios::floatfield);  // default
        for (int i = 0; i < samples; ++i) {
            auto [n, isp, t] = bench(rng, uniform, &cppr::IsPrimeNoTable);
            char buf[96];
            int len = std::snprintf(buf, sizeof(buf), "%llu,%d,%.12f\n", static_cast<unsigned long long>(n), isp ? 1 : 0, t);
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
    };

    if (heavy) {
        bench_IsPrime();
        bench_IsPrimeNoTable();
    } else {
        auto th1 = std::thread(bench_IsPrime);
        auto th2 = std::thread(bench_IsPrimeNoTable);
        th1.join();
        th2.join();
    }

    // Output summary
    std::ofstream summary("benchmarks/bench_summary.csv", std::ios::trunc);
    std::ostringstream summary_md;
    summary << "avg_time_prime_IsPrime,avg_time_prime_IsPrimeNoTable,avg_time_composite_IsPrime,avg_time_composite_IsPrimeNoTable\n";
    summary << std::fixed << std::setprecision(6);
    summary_md << std::fixed << std::setprecision(2);
    auto print_result = [](auto& f, double val, std::int32_t count) -> auto& {
        if (count) {
            f << (val / count);
        } else {
            f << "nan";
        }
        return f;
    };
    auto average_or_nan = [](double sum, std::int32_t count) -> double {
        if (count) {
            return sum / count;
        }
        return std::numeric_limits<double>::quiet_NaN();
    };
    auto range_average = [&](std::int32_t begin, std::int32_t end, const double* sums, const std::int32_t* counts) -> double {
        double total = 0.0;
        std::int32_t used = 0;
        for (std::int32_t i = begin; i <= end; ++i) {
            double avg = average_or_nan(sums[i], counts[i]);
            if (avg == avg) {
                total += avg;
                ++used;
            }
        }
        return used ? (total / used) : std::numeric_limits<double>::quiet_NaN();
    };
    for (std::int32_t i = 1; i <= 64; ++i) {
        print_result(summary, time_prime_sum[i], count_prime[i]) << ",";
        print_result(summary, time_prime_sum_NoTable[i], count_prime_NoTable[i]) << ",";
        print_result(summary, time_composite_sum[i], count_composite[i]) << ",";
        print_result(summary, time_composite_sum_NoTable[i], count_composite_NoTable[i]) << "\n";
    }
    summary_md << "# Benchmark Summary\n\n";
    summary_md << "## Overall summary\n\n";
    summary_md << "- IsPrime averages " << range_average(1, 64, time_prime_sum, count_prime) << " ns on prime inputs and " << range_average(1, 64, time_composite_sum, count_composite)
               << " ns on composite inputs.\n";
    summary_md << "- IsPrimeNoTable averages " << range_average(1, 64, time_prime_sum_NoTable, count_prime_NoTable) << " ns on prime inputs and "
               << range_average(1, 64, time_composite_sum_NoTable, count_composite_NoTable) << " ns on composite inputs.\n\n";
    summary_md << "## Averages by 8-bit range (nanoseconds)\n\n";
    summary_md << "| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |\n";
    summary_md << "|-----------|-----------------|------------------------|---------------------|----------------------------|\n";
    const std::pair<std::int32_t, std::int32_t> ranges[] = {
        {1, 8}, {9, 16}, {17, 24}, {25, 32}, {33, 40}, {41, 48}, {49, 56}, {57, 62}, {63, 64},
    };
    for (std::size_t idx = 0; idx < sizeof(ranges) / sizeof(ranges[0]); ++idx) {
        const std::int32_t begin = ranges[idx].first;
        const std::int32_t end = ranges[idx].second;
        summary_md << "| " << begin << "-" << end << " | ";
        summary_md << range_average(begin, end, time_prime_sum, count_prime) << " | ";
        summary_md << range_average(begin, end, time_prime_sum_NoTable, count_prime_NoTable) << " | ";
        summary_md << range_average(begin, end, time_composite_sum, count_composite) << " | ";
        summary_md << range_average(begin, end, time_composite_sum_NoTable, count_composite_NoTable) << " |\n";
    }
    summary << std::flush;

    std::ofstream summary_md_file("benchmarks/bench_summary.md", std::ios::trunc);
    summary_md_file << summary_md.str() << std::flush;

    std::cout << "Benchmarking completed\n";
    std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_time).count() << " seconds\n\n";
    std::cout << summary_md.str() << std::flush;

    return 0;
}
