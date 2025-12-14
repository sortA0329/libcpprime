#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>
#include <primesieve/iterator.hpp>
#include <random>
#include <set>
#include <string>

struct IsPrimeImpl {
    static bool IsPrime(uint64_t n) { return cppr::IsPrime(n); }
};
struct IsPrimeNoTableImpl {
    static bool IsPrime(uint64_t n) { return cppr::IsPrimeNoTable(n); }
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

TYPED_TEST(IsPrimeTest, Zero) { ASSERT_FALSE(TypeParam::IsPrime(0)); }
TYPED_TEST(IsPrimeTest, One) { ASSERT_FALSE(TypeParam::IsPrime(1)); }

TYPED_TEST(IsPrimeTest, 24bit) {
    primesieve::iterator it;
    uint64_t prev = 2;
    uint64_t prime = it.next_prime();
    while (prime < (1ull << 24)) {
        for (uint64_t n = prev; n < prime; n++) {
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
        }
        ASSERT_TRUE(TypeParam::IsPrime(prime)) << "Failed for prime = " << prime;
        prev = prime + 1;
        prime = it.next_prime();
    }
}

TYPED_TEST(IsPrimeTest, 32bit) {
    primesieve::iterator it(1u << 24, UINT32_MAX);
    uint32_t count = 0;
    uint64_t prev = 1u << 24;
    std::mt19937_64 rng;
    while (true) {
        uint64_t prime = it.next_prime();
        if (count % 32 == 0) {
            ASSERT_TRUE(TypeParam::IsPrime(prime)) << "Failed for prime = " << prime;
        }
        if (count % 4 == 0) {
            uint64_t n = prev + rng() % (prime - prev);
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
        }
        count++;
        prev = prime + 1;
        if (prime == 4294967291u) break;  // last 32-bit prime
    }
}

TYPED_TEST(IsPrimeTest, Pow2) {
    for (uint32_t exp = 2; exp <= 63; exp++) {
        uint64_t n = 1ull << exp;
        ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
    }
}

TYPED_TEST(IsPrimeTest, Mersenne) {
    std::set<uint64_t> primes = {3, 7, 31, 127, 8191, 131071, 524287, 2147483647, 2305843009213693951};
    for (uint32_t exp = 1; exp <= 63; exp++) {
        uint64_t n = (2ull << exp) - 1;
        if (primes.contains(n)) {
            ASSERT_TRUE(TypeParam::IsPrime(n)) << "Failed for prime = " << n;
        } else {
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
        }
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
        uint64_t n = std::stoull(line);
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

TEST(IsPrimeTest, CompareImplementations) {
    std::mt19937_64 rng;
    for (uint64_t i = 0; i <= 20000000; i++) {
        uint64_t n = rng() >> (rng() % 32);
        ASSERT_EQ(cppr::IsPrime(n), cppr::IsPrimeNoTable(n)) << "Mismatch for n = " << n;
    }
}
