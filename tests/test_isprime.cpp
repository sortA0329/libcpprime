#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>
#include <random>
#include <set>
#include <span>
#include <string>
#include <type_traits>
#include <vector>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#include <primesieve.hpp>
#include <primesieve/iterator.hpp>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

struct IsPrimeImpl {
    constexpr static bool IsPrime(std::uint64_t n) { return cppr::IsPrime(n); }
};
struct IsPrimeNoTableImpl {
    constexpr static bool IsPrime(std::uint64_t n) { return cppr::IsPrimeNoTable(n); }
};
template <class Impl>
class IsPrimeTest : public ::testing::Test {};
using IsPrimeImplementations = ::testing::Types<IsPrimeImpl, IsPrimeNoTableImpl>;
struct IsPrimeImplName {
    template <class T>
    static std::string GetName(int) {
        if constexpr (std::is_same_v<T, IsPrimeImpl>) {
            return "IsPrime";
        } else if constexpr (std::is_same_v<T, IsPrimeNoTableImpl>) {
            return "IsPrimeNoTable";
        } else {
            return "Unknown";
        }
    }
};
TYPED_TEST_SUITE(IsPrimeTest, IsPrimeImplementations, IsPrimeImplName);

auto GetPrimes() {
    static std::vector<std::uint32_t> primes;
    static bool initialized = false;
    if (!initialized) {
        primesieve::generate_primes(4294967295u, &primes);
        initialized = true;
    }
    return std::span(primes.begin(), primes.end());
}

TYPED_TEST(IsPrimeTest, Zero) { ASSERT_FALSE(TypeParam::IsPrime(0)); }
TYPED_TEST(IsPrimeTest, One) { ASSERT_FALSE(TypeParam::IsPrime(1)); }

TYPED_TEST(IsPrimeTest, 24bit) {
    auto primes = GetPrimes();
    std::uint64_t next_index = 0;
    for (std::uint32_t n = 2; n < (1u << 24); n++) {
        if (n == primes[next_index]) {
            ASSERT_TRUE(TypeParam::IsPrime(n)) << "Failed for prime = " << n;
            next_index++;
        } else {
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
        }
    }
}

TYPED_TEST(IsPrimeTest, 32bit) {
    auto primes = GetPrimes();
    std::mt19937_64 rng;
    std::uint32_t start_index = static_cast<std::uint32_t>(std::lower_bound(primes.begin(), primes.end(), (1ull << 24)) - primes.begin());
    for (std::uint32_t i = start_index; i < primes.size(); i++) {
        std::uint64_t prime = primes[i];
        if (i % 64 == 0) {
            ASSERT_TRUE(TypeParam::IsPrime(prime)) << "Failed for prime = " << prime;
        }
        if (i % 4 == 0) {
            std::uint64_t prev = primes[i - 1];
            std::uint64_t n = prev + 1 + (rng() % (prime - prev - 1));
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
        }
    }
}

TYPED_TEST(IsPrimeTest, Pow2) {
    for (int exp = 2; exp <= 63; exp++) {
        std::uint64_t n = 1ull << exp;
        ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
    }
}

TYPED_TEST(IsPrimeTest, Mersenne) {
    std::set<std::uint64_t> mersenne_primes = {3, 7, 31, 127, 8191, 131071, 524287, 2147483647, 2305843009213693951};
    for (int exp = 1; exp <= 63; exp++) {
        std::uint64_t n = (2ull << exp) - 1;
        if (mersenne_primes.contains(n)) {
            ASSERT_TRUE(TypeParam::IsPrime(n)) << "Failed for prime = " << n;
        } else {
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
        }
    }
}

TYPED_TEST(IsPrimeTest, Fermat) {
    std::set<std::uint64_t> fermat_primes = {3, 5, 17, 257, 65537};
    for (int exp = 1; exp <= 63; exp++) {
        std::uint64_t n = (1ull << exp) + 1;
        if (fermat_primes.contains(n)) {
            ASSERT_TRUE(TypeParam::IsPrime(n)) << "Failed for prime = " << n;
        } else {
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
        }
    }
}

TYPED_TEST(IsPrimeTest, ProdTwoPrime) {
    auto primes = GetPrimes();
    std::mt19937_64 rng;
    std::uint32_t start_index = static_cast<std::uint32_t>(std::lower_bound(primes.begin(), primes.end(), (1ull << 16)) - primes.begin());
    for (int i = 0; i < 6000000; ++i) {
        std::uint64_t p1 = primes[start_index + (rng() % (primes.size() - start_index))];
        std::uint64_t p2 = primes[start_index + (rng() % (primes.size() - start_index))];
        std::uint64_t n = p1 * p2;
        ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
    }
}

template <class Impl>
void RunTestsFromFile(const std::string& filename, bool expected_result) {
    auto filepath = std::filesystem::path(__FILE__).parent_path() / (expected_result ? "primes" : "composites") / filename;
    std::ifstream file(filepath);
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << filepath;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::uint64_t n = std::stoull(line);
        if (expected_result) {
            ASSERT_TRUE(Impl::IsPrime(n)) << "Failed for prime = " << n;
        } else {
            ASSERT_FALSE(Impl::IsPrime(n)) << "Failed for composite = " << n;
        }
    }
}

TYPED_TEST(IsPrimeTest, 2_SPRP_32bit) { RunTestsFromFile<TypeParam>("2_SPRP_32bit.txt", false); }
TYPED_TEST(IsPrimeTest, 2_SPRP_62bit) { RunTestsFromFile<TypeParam>("2_SPRP_62bit.txt", false); }
TYPED_TEST(IsPrimeTest, 2_SPRP_64bit) { RunTestsFromFile<TypeParam>("2_SPRP_64bit.txt", false); }
TYPED_TEST(IsPrimeTest, carmichael) { RunTestsFromFile<TypeParam>("carmichael.txt", false); }
TYPED_TEST(IsPrimeTest, carmichael_strong_pseudoprimes) { RunTestsFromFile<TypeParam>("carmichael_strong_pseudoprimes.txt", false); }
TYPED_TEST(IsPrimeTest, hack_7_bases) { RunTestsFromFile<TypeParam>("hack_7_bases.txt", false); }
TYPED_TEST(IsPrimeTest, hack_8_prime_bases) { RunTestsFromFile<TypeParam>("hack_8_prime_bases.txt", false); }
TYPED_TEST(IsPrimeTest, hack_base_2_to_10) { RunTestsFromFile<TypeParam>("hack_base_2_to_10.txt", false); }
TYPED_TEST(IsPrimeTest, hack_known_bases) { RunTestsFromFile<TypeParam>("hack_known_bases.txt", false); }
TYPED_TEST(IsPrimeTest, strong_lucas_pseudoprimes) { RunTestsFromFile<TypeParam>("strong_lucas_pseudoprimes.txt", false); }
TYPED_TEST(IsPrimeTest, primes_64bit) { RunTestsFromFile<TypeParam>("primes_64bit.txt", true); }

TEST(IsPrimeTest, CompareImplementations) {
    std::mt19937_64 rng;
    for (uint64_t i = 0; i < 10000000; i++) {
        std::uint64_t n = (rng() >> (rng() % 32)) | 1;
        ASSERT_EQ(cppr::IsPrime(n), cppr::IsPrimeNoTable(n)) << "Mismatch for n = " << n;
    }
}

constexpr static std::uint64_t Primes[] = {
#include "./primes/constexpr.txt"
};
constexpr static std::uint64_t Composites[] = {
#include "./composites/constexpr.txt"
};
struct ConstexprTestResult {
    bool success;
    std::uint64_t value;
};
template <ConstexprTestResult T, bool Prime>
struct ConstexprTestMessage {
    static_assert(!Prime || T.success, "Constexpr prime test failed");
    static_assert(Prime || T.success, "Constexpr composite test failed");
};
TYPED_TEST(IsPrimeTest, ConstexprTest) {
    auto check_primes = []() {
        for (std::uint64_t prime : Primes) {
            if (!TypeParam::IsPrime(prime)) {
                return ConstexprTestResult{false, prime};
            }
        }
        return ConstexprTestResult{true, 0ull};
    };
    constexpr auto result_primes = check_primes();
    (void)ConstexprTestMessage<result_primes, true>{};
    auto check_composites = []() {
        for (std::uint64_t composite : Composites) {
            if (TypeParam::IsPrime(composite)) {
                return ConstexprTestResult{false, composite};
            }
        }
        return ConstexprTestResult{true, 0ull};
    };
    constexpr auto result_composites = check_composites();
    (void)ConstexprTestMessage<result_composites, false>{};
}
