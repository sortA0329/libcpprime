/**
 *
 * libcpprime https://github.com/Rac75116/libcpprime
 *
 * Copyright (c) 2025 Rac75116
 * SPDX-License-Identifier: MIT
 *
 **/
/**
 *
 * The algorithm in this library is based on Bradley Berg's method.
 * See this page for more information:
 *https://www.techneon.com/download/is.prime.64.base.data
 *
 * Copyright 2018 Bradley Berg   < (My last name) @ t e c h n e o n . c o m >
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * This algorithm is deliberately unpatented. The license above also
 * lets you even freely use it in commercial code.
 *
 * Primality testing using a hash table of bases originated with Steven Worley.
 *
 **/

#ifndef CPPR_INTERNAL_INCLUDED_IS_PRIME
#define CPPR_INTERNAL_INCLUDED_IS_PRIME

#include <cstdint>

#include "internal/IsPrimeCommon.hpp"
#include "internal/Utils.hpp"

namespace cppr {

namespace internal {

constexpr std::uint64_t FlagTable17[1024] = {
#include "internal/IsPrimeTable17.txt"
};
// Bitset for odd numbers < 2^17 (2 is handled explicitly).
CPPR_INTERNAL_CONSTEXPR_INLINE bool IsPrime17(const std::uint64_t n) noexcept { return n == 2 || (n % 2 == 1 && (FlagTable17[n / 128] & (1ull << (n % 128 / 2)))); }

constexpr std::uint16_t Bases64[16384] = {
#include "internal/IsPrimeBases64.txt"
};
// Deterministic base selection via a multiplicative hash (fast table lookup).
CPPR_INTERNAL_CONSTEXPR_INLINE std::uint16_t GetBase(std::uint64_t x) noexcept { return Bases64[(0xad625b89u * static_cast<std::uint32_t>(x)) >> 18]; }
CPPR_INTERNAL_CONSTEXPR_INLINE bool IsPrime49(const std::uint64_t x) noexcept {
    const MontgomeryModint64Impl<false> mint(x);
    const std::int32_t S = CountrZero(x - 1);
    const std::uint64_t D = (x - 1) >> S;
    const auto one = mint.one();
    const auto mone = mint.neg(one);
    auto c = mint.raw(2);
    auto d = mint.raw(GetBase(x));
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
    // For x % 4 == 1 we may need the full Miller-Rabin squaring chain;
    // for x % 4 == 3 the initial check is sufficient with the selected bases.
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
template <bool Strict>
CPPR_INTERNAL_CONSTEXPR_INLINE bool IsPrime64(const std::uint64_t x) noexcept {
    const MontgomeryModint64Impl<Strict> mint(x);
    const std::int32_t S = CountrZero(x - 1);
    const std::uint64_t D = (x - 1) >> S;
    const auto one = mint.one();
    const auto mone = mint.neg(one);
    const std::uint32_t base = GetBase(x);
    // Third base is packed as a small lookup indexed by the high bits of `base`.
    const std::uint64_t base_mask = static_cast<std::uint64_t>(15ull | (135ull << 8) | (13ull << 16) | (60ull << 24) | (15ull << 32) | (117ull << 40) | (65ull << 48) | (29ull << 56));
    auto d = mint.raw(2);
    auto e = mint.raw(base);
    auto f = mint.raw((base_mask >> (8 * (base >> 13))) & 0xff);
    auto a = d;
    auto b = e;
    auto c = f;
    if (D != 1) {
        d = mint.mul(d, d);
        e = mint.mul(e, e);
        f = mint.mul(f, f);
        std::uint64_t ex = D >> 1;
        while (ex != 1) {
            const auto g = mint.mul(d, d);
            const auto h = mint.mul(e, e);
            const auto i = mint.mul(f, f);
            if (ex & 1) {
                a = mint.mul(a, d);
                b = mint.mul(b, e);
                c = mint.mul(c, f);
            }
            d = g;
            e = h;
            f = i;
            ex >>= 1;
        }
    }
    a = mint.mul(a, d);
    b = mint.mul(b, e);
    c = mint.mul(c, f);
    bool res1 = mint.same(a, one) || mint.same(a, mone);
    bool res2 = mint.same(b, one) || mint.same(b, mone);
    bool res3 = mint.same(c, one) || mint.same(c, mone);
    // Same idea as IsPrime49: only run the MR squaring chain when needed.
    if (x % 4 == 1 && !(res1 && res2 && res3)) {
        for (std::int32_t i = 0; i != S - 1; ++i) {
            a = mint.mul(a, a);
            b = mint.mul(b, b);
            c = mint.mul(c, c);
            res1 |= mint.same(a, mone);
            res2 |= mint.same(b, mone);
            res3 |= mint.same(c, mone);
        }
    }
    return res1 && res2 && res3;
}

}  // namespace internal

CPPR_INTERNAL_CONSTEXPR bool IsPrime(std::uint64_t n) noexcept {
    if (n < 131072) {
        return internal::IsPrime17(n);
    } else if (n <= 0xffffffff) {
        if (internal::TrialDivision32(static_cast<std::uint32_t>(n))) return false;
        return internal::IsPrime32(static_cast<std::uint32_t>(n));
    } else {
        // Cheap small-prime screening before the heavier probable-prime tests.
        if (internal::TrialDivision64(n)) {
            return false;
        }
        if (n < (std::uint64_t(1) << 49)) {
            return internal::IsPrime49(n);
        } else if (n < (std::uint64_t(1) << 62)) {
            return internal::IsPrime64<false>(n);
        } else {
            // Strict variant keeps Montgomery values within [0, mod) for the largest inputs.
            return internal::IsPrime64<true>(n);
        }
    }
}

}  // namespace cppr

#endif
