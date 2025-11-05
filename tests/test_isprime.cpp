#include <gtest/gtest.h>

#include <cstdint>
#include <fstream>
#include <libcpprime/IsPrime.hpp>
#include <libcpprime/IsPrimeNoTable.hpp>
#include <string>
#include <vector>

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

TEST_F(IsPrimeTest, DatafilePrimes) {
    if (primes.empty()) {
        GTEST_SKIP() << "Primes.txt is empty, skipping datafile test";
    }
    for (const auto& prime : primes) {
        EXPECT_TRUE(cppr::IsPrime(prime)) << "Failed for prime: " << prime;
        EXPECT_TRUE(cppr::IsPrimeNoTable(prime)) << "Failed for prime: " << prime;
    }
}

TEST_F(IsPrimeTest, DatafileComposites) {
    if (composites.empty()) {
        GTEST_SKIP() << "Composites.txt is empty, skipping datafile test";
    }
    for (const auto& composite : composites) {
        EXPECT_FALSE(cppr::IsPrime(composite)) << "Failed for composite: " << composite;
        EXPECT_FALSE(cppr::IsPrimeNoTable(composite)) << "Failed for composite: " << composite;
    }
}
