/**
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

#include <cstdint>

#include "./internal/IsPrimeCommon.hpp"

#ifndef LIBCPPRIME_INCLUDED_IS_PRIME
#define LIBCPPRIME_INCLUDED_IS_PRIME

namespace cppr {

namespace internal {

constexpr std::uint64_t FlagTable16[512] = {
#include "./internal/IsPrimeTable16.txt"
};
LIBCPPRIME_CONSTEXPR inline bool IsPrime16(const std::uint64_t n) noexcept { return n == 2 || (n % 2 == 1 && (FlagTable16[n / 128] & (1ull << (n % 128 / 2)))); }

constexpr const std::uint16_t Bases64[16384] = {
#include "./internal/IsPrimeBases64.txt"
};
template <bool Strict>
LIBCPPRIME_CONSTEXPR inline bool IsPrime64(const std::uint64_t x) noexcept {
    const MontgomeryModint64Impl<Strict> mint(x);
    const std::int32_t S = CountrZero(x - 1);
    const std::uint64_t D = (x - 1) >> S;
    const auto one = mint.one();
    const auto mone = mint.neg(one);
    auto test2 = [=](std::uint64_t base1, std::uint64_t base2) -> bool {
        auto c = mint.raw(base1);
        auto d = mint.raw(base2);
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
    };
    auto test3 = [=](std::uint64_t base1, std::uint64_t base2, std::uint64_t base3) -> bool {
        auto d = mint.raw(base1);
        auto e = mint.raw(base2);
        auto f = mint.raw(base3);
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
    };
    const std::uint32_t base = Bases64[(0xad625b89u * static_cast<std::uint32_t>(x)) >> 18];
    if (x < (1ull << 49)) {
        return test2(2, base);
    } else {
        const std::uint64_t base2_mask = 15ull | (135ull << 8) | (13ull << 16) | (60ull << 24) | (15ull << 32) | (117ull << 40) | (65ull << 48) | (29ull << 56);
        return test3(2, base, (base2_mask >> (8 * (base >> 13))) & 0xff);
    }
}

}  // namespace internal

LIBCPPRIME_CONSTEXPR inline bool IsPrime(std::uint64_t n) noexcept {
    if (n < 65536) {
        return internal::IsPrime16(n);
    } else {
        if (internal::TrialDivision(n)) {
            return false;
        }
        if (n <= 0xffffffff) {
            return internal::IsPrime32(static_cast<std::uint32_t>(n));
        } else if (n < (std::uint64_t(1) << 62)) {
            return internal::IsPrime64<false>(n);
        } else {
            return internal::IsPrime64<true>(n);
        }
    }
}

}  // namespace cppr

#endif
