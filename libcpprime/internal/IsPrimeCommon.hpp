/**
 * libcpprime https://github.com/sortA0329/libcpprime
 * 
 * MIT License
 *
 * Copyright (c) 2024 Rac
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/
#ifndef LIBCPPRIME_INCLUDED_IS_PRIME_COMMON
#define LIBCPPRIME_INCLUDED_IS_PRIME_COMMON

#include <cstdint>
#ifdef __has_include
#if __has_include(<type_traits>)
#include <type_traits>
#endif
#if __has_include(<bit>)
#include <bit>
#endif
#endif
#ifdef _MSC_VER
#include <intrin.h>
#include <immintrin.h>
#pragma intrinsic(__assume, _tzcnt_u64, _umul128, __umulh)
#if _MSC_VER >= 1900
#pragma intrinsic(_udiv128)
#endif
#endif
#if defined(__cpp_lib_is_constant_evaluated) && defined(__cpp_lib_bitops)
#define LIBCPPRIME_CONSTEXPR constexpr
#else
#define LIBCPPRIME_CONSTEXPR
#endif
#ifdef __cpp_if_constexpr
#define LIBCPPRIME_IF_CONSTEXPR constexpr
#else
#define LIBCPPRIME_IF_CONSTEXPR
#endif

namespace cppr {

namespace internal {

    struct Int64Pair {
        std::uint64_t high, low;
    };
    LIBCPPRIME_CONSTEXPR void Assume(const bool f) noexcept {
#ifdef __cpp_lib_is_constant_evaluated
        if (std::is_constant_evaluated()) return;
#endif
#if defined(__clang__)
#if __has_builtin(__builtin_assume)
        __builtin_assume(f);
#endif
#elif defined(__GNUC__)
        if (!f) __builtin_unreachable();
#elif defined(_MSC_VER)
        __assume(f);
#endif
    }
    LIBCPPRIME_CONSTEXPR std::int32_t CountrZero(std::uint64_t n) noexcept {
#ifdef __cpp_lib_bitops
        Assume(n != 0);
        return std::countr_zero(n);
#elif defined(__GNUC__)
        return __builtin_ctzll(n);
#elif defined(_MSC_VER)
        return _tzcnt_u64(n);
#else
        std::uint64_t x = (n ^ (n - 1)) >> 1;
        x = x - ((x >> 1) & 0x5555555555555555);
        x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
        x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
        x = x + (x >> 8);
        x = x + (x >> 16);
        x = x + (x >> 32);
        return x & 0x0000007f;
#endif
    }
    LIBCPPRIME_CONSTEXPR Int64Pair Mulu128(std::uint64_t muler, std::uint64_t mulnd) noexcept {
#if defined(__SIZEOF_INT128__)
        __uint128_t tmp = static_cast<__uint128_t>(muler) * mulnd;
        return { static_cast<std::uint64_t>(tmp >> 64), static_cast<std::uint64_t>(tmp) };
#else
#if defined(_MSC_VER)
#ifdef __cpp_lib_is_constant_evaluated
        if (!std::is_constant_evaluated())
#endif
        {
            std::uint64_t high;
            std::uint64_t low = _umul128(muler, mulnd, &high);
            return { high, low };
        }
#endif
        std::uint64_t u1 = (muler & 0xffffffff);
        std::uint64_t v1 = (mulnd & 0xffffffff);
        std::uint64_t t = (u1 * v1);
        std::uint64_t w3 = (t & 0xffffffff);
        std::uint64_t k = (t >> 32);
        muler >>= 32;
        t = (muler * v1) + k;
        k = (t & 0xffffffff);
        std::uint64_t w1 = (t >> 32);
        mulnd >>= 32;
        t = (u1 * mulnd) + k;
        k = (t >> 32);
        return { (muler * mulnd) + w1 + k, (t << 32) + w3 };
#endif
    }
    LIBCPPRIME_CONSTEXPR
    std::uint64_t Mulu128High(std::uint64_t muler, std::uint64_t mulnd) noexcept {
#if defined(__SIZEOF_INT128__)
        return static_cast<std::uint64_t>((static_cast<__uint128_t>(muler) * mulnd) >> 64);
#else
#if defined(_MSC_VER)
#ifdef __cpp_lib_is_constant_evaluated
        if (!std::is_constant_evaluated())
#endif
            return __umulh(muler, mulnd);
#endif
        return Mulu128(muler, mulnd).high;
#endif
    }
    LIBCPPRIME_CONSTEXPR Int64Pair Divu128(std::uint64_t high, std::uint64_t low, std::uint64_t div) noexcept {
#if (defined(__GNUC__) || defined(__ICC)) && defined(__x86_64__)
        if LIBCPPRIME_IF_CONSTEXPR (sizeof(void*) == 8) {
#ifdef __cpp_lib_is_constant_evaluated
            if (!std::is_constant_evaluated())
#endif
            {
                std::uint64_t res = 0, rem = 0;
                __asm__("divq %[v]" : "=a"(res), "=d"(rem) : [v] "r"(div), "a"(low), "d"(high));
                return { res, rem };
            }
        }
#elif defined(_MSC_VER) && _MSC_VER >= 1900
#ifdef __cpp_lib_is_constant_evaluated
        if (!std::is_constant_evaluated())
#endif
        {
            std::uint64_t rem = 0;
            std::uint64_t res = _udiv128(high, low, div, &rem);
            return { res, rem };
        }
#endif
#if defined(__SIZEOF_INT128__)
        __uint128_t n = (static_cast<__uint128_t>(high) << 64 | low);
        __uint128_t res = n / div;
        return { static_cast<std::uint64_t>(res), static_cast<std::uint64_t>(n - res * div) };
#else
        std::uint64_t res = 0;
        std::uint64_t cur = high;
        for (std::uint64_t i = 0; i != 64; ++i) {
            std::uint64_t large = cur >> 63;
            cur = cur << 1 | (low >> 63);
            low <<= 1;
            large |= (cur >= div);
            res = res << 1 | large;
            cur -= div & (0 - large);
        }
        return { res, cur };
#endif
    }

    template<bool Strict = false> class MontgomeryModint64Impl {
        std::uint64_t mod_ = 0, rs = 0, nr = 0, np = 0;
        LIBCPPRIME_CONSTEXPR std::uint64_t reduce(const std::uint64_t n) const noexcept {
            std::uint64_t q = n * nr;
            if LIBCPPRIME_IF_CONSTEXPR (Strict) {
                auto m = Mulu128(q, mod_);
                std::uint64_t t = m.high + std::uint64_t(m.low + n < m.low);
                return t - mod_ * (t >= mod_);
            } else {
                std::uint64_t m = Mulu128High(q, mod_);
                return mod_ - m;
            }
        }
        LIBCPPRIME_CONSTEXPR std::uint64_t reduce(const std::uint64_t a, const std::uint64_t b) const noexcept {
            auto tmp = Mulu128(a, b);
            std::uint64_t d = tmp.high, c = tmp.low;
            std::uint64_t q = c * nr;
            if LIBCPPRIME_IF_CONSTEXPR (Strict) {
                auto m = Mulu128(q, mod_);
                std::uint64_t t = m.high + std::uint64_t(m.low + c < m.low);
                if (t >= mod_ - d) return t + d - mod_;
                else return t + d;
            } else {
                std::uint64_t m = Mulu128High(q, mod_);
                return d + mod_ - m;
            }
        }

    public:
        LIBCPPRIME_CONSTEXPR MontgomeryModint64Impl() noexcept {}
        LIBCPPRIME_CONSTEXPR void set(std::uint64_t n) noexcept {
            Assume(n > 2 && n % 2 != 0);
            mod_ = n;
            rs = Divu128(0xffffffffffffffff % n, 0 - n, n).low;
            nr = n;
            for (std::uint32_t i = 0; i != 6; ++i) nr *= 2 - n * nr;
            if LIBCPPRIME_IF_CONSTEXPR (Strict) nr = 0 - nr;
            np = reduce(rs);
        }
        LIBCPPRIME_CONSTEXPR std::uint64_t build(std::uint32_t x) const noexcept { return reduce(x % mod_, rs); }
        LIBCPPRIME_CONSTEXPR std::uint64_t build(std::uint64_t x) const noexcept { return reduce(x % mod_, rs); }
        LIBCPPRIME_CONSTEXPR std::uint64_t raw(std::uint64_t x) const noexcept {
            Assume(x < mod_);
            return reduce(x, rs);
        }
        LIBCPPRIME_CONSTEXPR std::uint64_t one() const noexcept {
            if LIBCPPRIME_IF_CONSTEXPR (Strict) {
                Assume(np < mod_);
                return np;
            } else {
                Assume(np < 2 * mod_);
                return np;
            }
        }
        LIBCPPRIME_CONSTEXPR std::uint64_t neg(std::uint64_t x) const noexcept {
            if LIBCPPRIME_IF_CONSTEXPR (Strict) {
                Assume(x < mod_);
                return (mod_ - x) * (x != 0);
            } else {
                Assume(x < 2 * mod_);
                return (2 * mod_ - x) * (x != 0);
            }
        }
        LIBCPPRIME_CONSTEXPR std::uint64_t mul(std::uint64_t x, std::uint64_t y) const noexcept {
            if LIBCPPRIME_IF_CONSTEXPR (Strict) {
                Assume(x < mod_ && y < mod_);
                return reduce(x, y);
            } else {
                Assume(x < 2 * mod_ && y < 2 * mod_);
                return reduce(x, y);
            }
        }
        LIBCPPRIME_CONSTEXPR bool same(std::uint64_t x, std::uint64_t y) const noexcept {
            if LIBCPPRIME_IF_CONSTEXPR (Strict) {
                Assume(x < mod_ && y < mod_);
                return x == y;
            } else {
                Assume(x < 2 * mod_ && y < 2 * mod_);
                std::uint64_t tmp = x - y;
                return (tmp == 0) || (tmp == mod_) || (tmp == 0 - mod_);
            }
        }
    };

    // clang-format off
	LIBCPPRIME_CONSTEXPR const std::uint16_t Bases[256] = {
1216,1836,8885,4564,10978,5228,15613,13941,1553,173,3615,3144,10065,9259,233,2362,6244,6431,10863,5920,6408,6841,22124,2290,45597,6935,4835,7652,1051,445,5807,842,1534,22140,1282,1733,347,6311,14081,11157,186,703,9862,15490,1720,17816,10433,49185,2535,9158,2143,2840,664,29074,24924,1035,41482,1065,10189,8417,130,4551,5159,48886,
786,1938,1013,2139,7171,2143,16873,188,5555,42007,1045,3891,2853,23642,148,3585,3027,280,3101,9918,6452,2716,855,990,1925,13557,1063,6916,4965,4380,587,3214,1808,1036,6356,8191,6783,14424,6929,1002,840,422,44215,7753,5799,3415,231,2013,8895,2081,883,3855,5577,876,3574,1925,1192,865,7376,12254,5952,2516,20463,186,
5411,35353,50898,1084,2127,4305,115,7821,1265,16169,1705,1857,24938,220,3650,1057,482,1690,2718,4309,7496,1515,7972,3763,10954,2817,3430,1423,714,6734,328,2581,2580,10047,2797,155,5951,3817,54850,2173,1318,246,1807,2958,2697,337,4871,2439,736,37112,1226,527,7531,5418,7242,2421,16135,7015,8432,2605,5638,5161,11515,14949,
748,5003,9048,4679,1915,7652,9657,660,3054,15469,2910,775,14106,1749,136,2673,61814,5633,1244,2567,4989,1637,1273,11423,7974,7509,6061,531,6608,1088,1627,160,6416,11350,921,306,18117,1238,463,1722,996,3866,6576,6055,130,24080,7331,3922,8632,2706,24108,32374,4237,15302,287,2296,1220,20922,3350,2089,562,11745,163,11951 };
    // clang-format on
    LIBCPPRIME_CONSTEXPR bool IsPrime32(const std::uint32_t x) noexcept {
        const std::uint32_t h = x * 0xad625b89;
        std::uint32_t d = x - 1;
        std::uint32_t pw = static_cast<std::uint32_t>(Bases[h >> 24]);
        std::uint32_t s = CountrZero(d);
        d >>= s;
        std::uint32_t cur = 1;
        while (d) {
            std::uint32_t tmp = std::uint64_t(pw) * pw % x;
            if (d & 1) cur = std::uint64_t(cur) * pw % x;
            pw = tmp;
            d >>= 1;
        }
        if (cur == 1 || cur == x - 1) return true;
        while (--s) {
            cur = std::uint64_t(cur) * cur % x;
            if (cur == x - 1) return true;
        }
        return false;
    }

}  // namespace internal

}  // namespace cppr

#endif
