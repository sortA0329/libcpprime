#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <libcpprime/FeatureTestMacros.hpp>
#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>
#include <random>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

#ifdef CPPR_HAS_CONSTEXPR_IS_PRIME
struct IsPrimeImpl {
    constexpr static bool IsPrime(std::uint64_t n) { return cppr::IsPrime(n); }
};
struct IsPrimeNoTableImpl {
    constexpr static bool IsPrime(std::uint64_t n) { return cppr::IsPrimeNoTable(n); }
};
#else
struct IsPrimeImpl {
    inline static bool IsPrime(std::uint64_t n) { return cppr::IsPrime(n); }
};
struct IsPrimeNoTableImpl {
    inline static bool IsPrime(std::uint64_t n) { return cppr::IsPrimeNoTable(n); }
};
#endif
template <class Impl>
class IsPrimeTest : public ::testing::Test {};
using IsPrimeImplementations = ::testing::Types<IsPrimeImpl, IsPrimeNoTableImpl>;
struct IsPrimeImplName {
    template <class T>
    static std::string GetName(int) {
        if (std::is_same<T, IsPrimeImpl>::value) {
            return "IsPrime";
        } else if (std::is_same<T, IsPrimeNoTableImpl>::value) {
            return "IsPrimeNoTable";
        } else {
            return "Unknown";
        }
    }
};
TYPED_TEST_SUITE(IsPrimeTest, IsPrimeImplementations, IsPrimeImplName);

TYPED_TEST(IsPrimeTest, Zero) { ASSERT_FALSE(TypeParam::IsPrime(0)); }
TYPED_TEST(IsPrimeTest, One) { ASSERT_FALSE(TypeParam::IsPrime(1)); }

const std::vector<bool>& GetPrimes() {
    static std::vector<bool> flags;
    if (flags.empty()) {
        constexpr std::uint64_t limit = 1u << 24;
        flags.resize(limit, true);
        flags[0] = false;
        flags[1] = false;
        for (std::uint64_t i = 2; i * i < limit; i++) {
            if (!flags[i]) continue;
            for (std::uint64_t j = i * i; j < limit; j += i) {
                flags[j] = false;
            }
        }
    }
    return flags;
}

TYPED_TEST(IsPrimeTest, 24bit) {
    const auto& flags = GetPrimes();
    for (std::uint64_t n = 0; n < flags.size(); n++) {
        if (flags[n]) {
            ASSERT_TRUE(TypeParam::IsPrime(n)) << "Failed for prime = " << n;
        } else {
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
        if (mersenne_primes.count(n)) {
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
        if (fermat_primes.count(n)) {
            ASSERT_TRUE(TypeParam::IsPrime(n)) << "Failed for prime = " << n;
        } else {
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n;
        }
    }
}

std::vector<std::uint64_t> ReadFile(const std::string& filepath, bool expected_result) {
    std::string current_file = __FILE__;
    std::string dir = current_file.substr(0, current_file.find_last_of("/\\"));
    std::string full_path = dir + "/" + (expected_result ? "primes" : "composites") + "/" + filepath;
    std::vector<std::uint64_t> numbers;
    std::ifstream file(full_path);
    if (!file.is_open()) {
        EXPECT_TRUE(file.is_open()) << "Failed to open file: " << full_path;
        return numbers;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::uint64_t n = std::stoull(line);
        numbers.push_back(n);
    }
    return numbers;
}

template <class Impl>
void RunTestsFromFile(const std::string& filename, bool expected_result) {
    std::vector<std::uint64_t> numbers = ReadFile(filename, expected_result);
    for (const auto n : numbers) {
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
TYPED_TEST(IsPrimeTest, mult2power1_composites) { RunTestsFromFile<TypeParam>("mult2power1.txt", false); }
TYPED_TEST(IsPrimeTest, primes_32bit) { RunTestsFromFile<TypeParam>("primes_32bit.txt", true); }
TYPED_TEST(IsPrimeTest, primes_64bit) { RunTestsFromFile<TypeParam>("primes_64bit.txt", true); }
TYPED_TEST(IsPrimeTest, mult2power1_primes) { RunTestsFromFile<TypeParam>("mult2power1.txt", true); }

TEST(IsPrimeTest, CompareImplementations) {
    std::mt19937_64 rng;
    for (std::uint64_t i = 0; i < 10000000; i++) {
        std::uint64_t n = (rng() >> (rng() % 40)) | 1;
        ASSERT_EQ(cppr::IsPrime(n), cppr::IsPrimeNoTable(n)) << "Mismatch for n = " << n;
    }
}

TYPED_TEST(IsPrimeTest, ProdTwoPrimes48bit) {
    const auto& flags = GetPrimes();
    for (std::uint64_t i = 2; i < flags.size(); i++) {
        if (!flags[i]) continue;
        for (std::uint64_t j = i; j < flags.size(); j++) {
            if (!flags[j]) continue;
            std::uint64_t n = i * j;
            if (n >= flags.size()) break;
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n << " = " << i << " * " << j;
        }
    }
}

TYPED_TEST(IsPrimeTest, ProdTwoPrimes64bit) {
    auto primes = ReadFile("primes_32bit.txt", true);
    for (std::size_t i = 0; i < primes.size(); i++) {
        for (std::size_t j = i; j < primes.size(); j++) {
            std::uint64_t n = primes[i] * primes[j];
            ASSERT_FALSE(TypeParam::IsPrime(n)) << "Failed for composite = " << n << " = " << primes[i] << " * " << primes[j];
        }
    }
}

TYPED_TEST(IsPrimeTest, Random) {
    std::uint64_t state = 0x123456789ABCDEF0ull;
    auto xorshift64star = [&state]() {
        state ^= state >> 12;
        state ^= state << 25;
        state ^= state >> 27;
        return state * 0x2545F4914F6CDD1Dull;
    };
    std::int32_t count_primes = 0;
    for (std::int32_t i = 0; i < 10000000; i++) {
        std::uint64_t n = xorshift64star() | 1;
        if (TypeParam::IsPrime(n)) {
            count_primes++;
        }
    }
    ASSERT_EQ(count_primes, 460802) << "Mismatch in number of primes found";
}

#ifdef CPPR_HAS_CONSTEXPR_IS_PRIME
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
    auto check_composites = []() {
        for (std::uint64_t composite : Composites) {
            if (TypeParam::IsPrime(composite)) {
                return ConstexprTestResult{false, composite};
            }
        }
        return ConstexprTestResult{true, 0ull};
    };
    constexpr auto result_primes = check_primes();
    (void)ConstexprTestMessage<result_primes, true>{};
    constexpr auto result_composites = check_composites();
    (void)ConstexprTestMessage<result_composites, false>{};
}
#endif
