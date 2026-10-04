/**
 *
 * libcpprime https://github.com/sortA0329/libcpprime
 *
 * Copyright (c) 2026 sortA
 * SPDX-License-Identifier: MIT
 *
 **/

#ifndef CPPR_INTERNAL_INCLUDED_IS_PRIME
#define CPPR_INTERNAL_INCLUDED_IS_PRIME

#include <cstdint>

#include "internal/Environment.hpp"
#include "internal/IsPrimeCommon.hpp"
#include "internal/Utils.hpp"

namespace cppr {

namespace internal {

constexpr std::uint64_t FlagTable21[16384] = {
#include "internal/IsPrimeTable21.txt"
};
CPPR_INTERNAL_CONSTEXPR_INLINE bool IsPrime21(const std::uint64_t n) noexcept { return n == 2 || (n % 2 == 1 && (FlagTable21[n / 128] & (1ull << (n % 128 / 2)))); }

constexpr std::uint16_t Bases64[262144] = {
#include "internal/IsPrimeBases64.txt"
};
template <bool Strict>
CPPR_INTERNAL_CONSTEXPR_INLINE bool IsPrime64(const std::uint64_t x) noexcept {
    const MontgomeryModint64Impl<Strict> mint(x);
    const std::int32_t S = CountrZero(x - 1);
    const std::uint64_t D = (x - 1) >> S;
    const auto one = mint.one();
    const auto mone = mint.mone();
    auto c = mint.raw(2);
    auto d = mint.raw(Bases64[(0xad625b89u * static_cast<std::uint32_t>(x)) >> 14]);
    auto a = c;
    auto b = d;
    if (D != 1) {
        c = mint.mul(c, c);
        d = mint.mul(d, d);
        std::uint64_t ex = D >> 1;
        while (ex != 1) {
            const auto e = mint.mul(c, c);
            const auto f = mint.mul(d, d);
            if (ex & 1) {
                a = mint.mul(a, c);
                b = mint.mul(b, d);
            }
            c = e;
            d = f;
            ex >>= 1;
        }
    }
    a = mint.mul(a, c);
    b = mint.mul(b, d);
    bool res1 = mint.same(a, one) || mint.same(a, mone);
    bool res2 = mint.same(b, one) || mint.same(b, mone);
    if (x % 4 == 1 && !(res1 && res2)) {
        for (std::int32_t i = 0; i != S - 1; ++i) {
            a = mint.mul(a, a);
            b = mint.mul(b, b);
            res1 |= mint.same(a, mone);
            res2 |= mint.same(b, mone);
        }
    }
    return res1 && res2;
}

}  // namespace internal

CPPR_INTERNAL_CONSTEXPR bool IsPrime(std::uint64_t n) noexcept {
    if (n < (1ull << 21)) {
        return internal::IsPrime21(n);
    } else if (n <= 0xffffffff) {
        if (internal::TrialDivision32(static_cast<std::uint32_t>(n))) return false;
        return internal::IsPrime32(static_cast<std::uint32_t>(n));
    } else {
        if (internal::TrialDivision64(n)) return false;
        if (n < (std::uint64_t(1) << 62)) {
            return internal::IsPrime64<false>(n);
        } else {
            return internal::IsPrime64<true>(n);
        }
    }
}

}  // namespace cppr

#endif
