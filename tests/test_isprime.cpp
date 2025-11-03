#include <gtest/gtest.h>
#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

class IsPrimeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Load test primes from file
        std::ifstream primes_file("tests/Primes.txt");
        if (primes_file.is_open()) {
            std::string line;
            while (std::getline(primes_file, line)) {
                if (!line.empty()) {
                    primes.push_back(std::stoull(line));
                }
            }
        }

        // Load test composites from file
        std::ifstream composites_file("tests/Composites.txt");
        if (composites_file.is_open()) {
            std::string line;
            while (std::getline(composites_file, line)) {
                if (!line.empty()) {
                    composites.push_back(std::stoull(line));
                }
            }
        }
    }

    std::vector<std::uint64_t> primes;
    std::vector<std::uint64_t> composites;
};

// Basic tests with known small primes and composites
TEST(IsPrimeBasicTest, SmallPrimes) {
    EXPECT_TRUE(cppr::IsPrime(2));
    EXPECT_TRUE(cppr::IsPrime(3));
    EXPECT_TRUE(cppr::IsPrime(5));
    EXPECT_TRUE(cppr::IsPrime(7));
    EXPECT_TRUE(cppr::IsPrime(11));
    EXPECT_TRUE(cppr::IsPrime(13));
    EXPECT_TRUE(cppr::IsPrime(17));
    EXPECT_TRUE(cppr::IsPrime(19));
    EXPECT_TRUE(cppr::IsPrime(23));
    EXPECT_TRUE(cppr::IsPrime(29));
}

TEST(IsPrimeBasicTest, SmallComposites) {
    EXPECT_FALSE(cppr::IsPrime(0));
    EXPECT_FALSE(cppr::IsPrime(1));
    EXPECT_FALSE(cppr::IsPrime(4));
    EXPECT_FALSE(cppr::IsPrime(6));
    EXPECT_FALSE(cppr::IsPrime(8));
    EXPECT_FALSE(cppr::IsPrime(9));
    EXPECT_FALSE(cppr::IsPrime(10));
    EXPECT_FALSE(cppr::IsPrime(12));
    EXPECT_FALSE(cppr::IsPrime(14));
    EXPECT_FALSE(cppr::IsPrime(15));
}

TEST(IsPrimeBasicTest, KnownLargePrimes) {
    EXPECT_TRUE(cppr::IsPrime(998244353));
    EXPECT_TRUE(cppr::IsPrime(1000000007));
}

TEST(IsPrimeBasicTest, KnownLargeComposites) {
    EXPECT_FALSE(cppr::IsPrime(999988224444335533ULL));
}

TEST_F(IsPrimeTest, DatafilePrimes) {
    if (primes.empty()) {
        GTEST_SKIP() << "Primes.txt is empty, skipping datafile test";
    }
    for (const auto& prime : primes) {
        EXPECT_TRUE(cppr::IsPrime(prime)) << "Failed for prime: " << prime;
    }
}

TEST_F(IsPrimeTest, DatafileComposites) {
    if (composites.empty()) {
        GTEST_SKIP() << "Composites.txt is empty, skipping datafile test";
    }
    for (const auto& composite : composites) {
        EXPECT_FALSE(cppr::IsPrime(composite)) << "Failed for composite: " << composite;
    }
}

// IsPrimeNoTable tests
class IsPrimeNoTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Load test primes from file
        std::ifstream primes_file("tests/Primes.txt");
        if (primes_file.is_open()) {
            std::string line;
            while (std::getline(primes_file, line)) {
                if (!line.empty()) {
                    primes.push_back(std::stoull(line));
                }
            }
        }

        // Load test composites from file
        std::ifstream composites_file("tests/Composites.txt");
        if (composites_file.is_open()) {
            std::string line;
            while (std::getline(composites_file, line)) {
                if (!line.empty()) {
                    composites.push_back(std::stoull(line));
                }
            }
        }
    }

    std::vector<std::uint64_t> primes;
    std::vector<std::uint64_t> composites;
};

TEST(IsPrimeNoTableBasicTest, SmallPrimes) {
    EXPECT_TRUE(cppr::IsPrimeNoTable(2));
    EXPECT_TRUE(cppr::IsPrimeNoTable(3));
    EXPECT_TRUE(cppr::IsPrimeNoTable(5));
    EXPECT_TRUE(cppr::IsPrimeNoTable(7));
    EXPECT_TRUE(cppr::IsPrimeNoTable(11));
    EXPECT_TRUE(cppr::IsPrimeNoTable(13));
    EXPECT_TRUE(cppr::IsPrimeNoTable(17));
    EXPECT_TRUE(cppr::IsPrimeNoTable(19));
    EXPECT_TRUE(cppr::IsPrimeNoTable(23));
    EXPECT_TRUE(cppr::IsPrimeNoTable(29));
}

TEST(IsPrimeNoTableBasicTest, SmallComposites) {
    EXPECT_FALSE(cppr::IsPrimeNoTable(0));
    EXPECT_FALSE(cppr::IsPrimeNoTable(1));
    EXPECT_FALSE(cppr::IsPrimeNoTable(4));
    EXPECT_FALSE(cppr::IsPrimeNoTable(6));
    EXPECT_FALSE(cppr::IsPrimeNoTable(8));
    EXPECT_FALSE(cppr::IsPrimeNoTable(9));
    EXPECT_FALSE(cppr::IsPrimeNoTable(10));
    EXPECT_FALSE(cppr::IsPrimeNoTable(12));
    EXPECT_FALSE(cppr::IsPrimeNoTable(14));
    EXPECT_FALSE(cppr::IsPrimeNoTable(15));
}

TEST(IsPrimeNoTableBasicTest, KnownLargePrimes) {
    EXPECT_TRUE(cppr::IsPrimeNoTable(998244353));
    EXPECT_TRUE(cppr::IsPrimeNoTable(1000000007));
}

TEST(IsPrimeNoTableBasicTest, KnownLargeComposites) {
    EXPECT_FALSE(cppr::IsPrimeNoTable(999988224444335533ULL));
}

TEST_F(IsPrimeNoTableTest, DatafilePrimes) {
    if (primes.empty()) {
        GTEST_SKIP() << "Primes.txt is empty, skipping datafile test";
    }
    for (const auto& prime : primes) {
        EXPECT_TRUE(cppr::IsPrimeNoTable(prime)) << "Failed for prime: " << prime;
    }
}

TEST_F(IsPrimeNoTableTest, DatafileComposites) {
    if (composites.empty()) {
        GTEST_SKIP() << "Composites.txt is empty, skipping datafile test";
    }
    for (const auto& composite : composites) {
        EXPECT_FALSE(cppr::IsPrimeNoTable(composite)) << "Failed for composite: " << composite;
    }
}

// Constexpr tests (C++20 only)
#if __cplusplus >= 202002L
constexpr bool RunConstexprTests() {
    // These tests run at compile-time in C++20
    static_assert(cppr::IsPrime(2));
    static_assert(cppr::IsPrime(3));
    static_assert(cppr::IsPrime(5));
    static_assert(!cppr::IsPrime(4));
    static_assert(!cppr::IsPrime(6));

    static_assert(cppr::IsPrimeNoTable(2));
    static_assert(cppr::IsPrimeNoTable(3));
    static_assert(cppr::IsPrimeNoTable(5));
    static_assert(!cppr::IsPrimeNoTable(4));
    static_assert(!cppr::IsPrimeNoTable(6));

    return true;
}

TEST(ConstexprTest, CompileTimeEvaluation) {
    constexpr bool result = RunConstexprTests();
    EXPECT_TRUE(result);
}
#endif
