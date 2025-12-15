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
 *https://www.techneon.com/download/is.prime.32.base.data
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
#include <immintrin.h>
#include <intrin.h>

#pragma intrinsic(_tzcnt_u64, _lzcnt_u64, _umul128, __umulh)
#if _MSC_VER >= 1900
#pragma intrinsic(_udiv128)
#endif
#endif
#if defined(__cpp_lib_is_constant_evaluated) && __cpp_constexpr >= 201907L
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

#if defined(__SIZEOF_INT128__)
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
using UInt128 = unsigned __int128;
#pragma GCC diagnostic pop
#else
using UInt128 = unsigned __int128;
#endif
#endif

struct Int64Pair {
    std::uint64_t high, low;
};
LIBCPPRIME_CONSTEXPR inline void Assume(const bool f) noexcept {
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
LIBCPPRIME_CONSTEXPR inline std::int32_t CountrZero(std::uint64_t n) noexcept {
#ifdef __cpp_lib_bitops
    Assume(n != 0);
    return std::countr_zero(n);
#elif defined(__GNUC__)
#ifdef __cpp_lib_is_constant_evaluated
    if (!std::is_constant_evaluated())
#endif
        return __builtin_ctzll(n);
#elif defined(_MSC_VER)
#ifdef __cpp_lib_is_constant_evaluated
    if (!std::is_constant_evaluated())
#endif
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
#if !defined(__cpp_lib_bitops) && !defined(__GNUC__) && !defined(_MSC_VER)
constexpr char CountlZeroTable[32] = {0, 31, 9, 30, 3, 8, 13, 29, 2, 5, 7, 21, 12, 24, 28, 19, 1, 10, 4, 14, 6, 22, 25, 20, 11, 15, 23, 26, 16, 27, 17, 18};
#endif
LIBCPPRIME_CONSTEXPR inline std::int32_t CountlZero(std::uint64_t n) noexcept {
#ifdef __cpp_lib_bitops
    Assume(n != 0);
    return std::countl_zero(n);
#elif defined(__GNUC__)
#ifdef __cpp_lib_is_constant_evaluated
    if (!std::is_constant_evaluated())
#endif
        return __builtin_clzll(n);
#elif defined(_MSC_VER)
#ifdef __cpp_lib_is_constant_evaluated
    if (!std::is_constant_evaluated())
#endif
        return _lzcnt_u64(n);
#else
    std::int32_t offset = 0;
    std::uint32_t x = n >> 32;
    if (n <= 0xffffffff) {
        offset = 32;
        x = n;
    }
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return offset + CountlZeroTable[(x * 0x076be629u) >> 27];
#endif
}
LIBCPPRIME_CONSTEXPR inline Int64Pair Mulu128(std::uint64_t muler, std::uint64_t mulnd) noexcept {
#if defined(__SIZEOF_INT128__)
    UInt128 tmp = static_cast<UInt128>(muler) * mulnd;
    return {static_cast<std::uint64_t>(tmp >> 64), static_cast<std::uint64_t>(tmp)};
#else
#if defined(_MSC_VER)
#ifdef __cpp_lib_is_constant_evaluated
    if (!std::is_constant_evaluated())
#endif
    {
        std::uint64_t high;
        std::uint64_t low = _umul128(muler, mulnd, &high);
        return {high, low};
    }
#endif
    std::uint64_t lhigh = muler >> 32;
    std::uint64_t llow = muler & 0xffffffff;
    std::uint64_t rhigh = mulnd >> 32;
    std::uint64_t rlow = mulnd & 0xffffffff;
    std::uint64_t mhh = lhigh * rhigh;
    std::uint64_t mll = llow * rlow;
    std::uint64_t mlh = llow * rhigh;
    std::uint64_t mhl = lhigh * rlow;
    std::uint64_t ma = mlh + mhl;
    mhh += ma >> 32;
    std::uint64_t rl = mll + (ma << 32);
    mhh += std::uint64_t(ma < mlh) << 32;
    mhh += rl < mll;
    return {mhh, rl};
#endif
}
LIBCPPRIME_CONSTEXPR inline std::uint64_t Mulu128High(std::uint64_t muler, std::uint64_t mulnd) noexcept {
#if defined(__SIZEOF_INT128__)
    return static_cast<std::uint64_t>((static_cast<UInt128>(muler) * mulnd) >> 64);
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
LIBCPPRIME_CONSTEXPR inline std::uint64_t Modu128(std::uint64_t high, std::uint64_t low, std::uint64_t div) noexcept {
#if (defined(__GNUC__) || defined(__ICC)) && defined(__x86_64__)
    if LIBCPPRIME_IF_CONSTEXPR (sizeof(void*) == 8) {
#ifdef __cpp_lib_is_constant_evaluated
        if (!std::is_constant_evaluated())
#endif
        {
            std::uint64_t quot = 0, rem = 0;
            __asm__("divq %[v]" : "=a"(quot), "=d"(rem) : [v] "r"(div), "a"(low), "d"(high));
            return rem;
        }
    }
#elif defined(_MSC_VER) && _MSC_VER >= 1900
#ifdef __cpp_lib_is_constant_evaluated
    if (!std::is_constant_evaluated())
#endif
    {
        std::uint64_t rem = 0;
        _udiv128(high, low, div, &rem);
        return rem;
    }
#endif
#if defined(__SIZEOF_INT128__)
    UInt128 n = (static_cast<UInt128>(high) << 64 | low);
    std::uint64_t quot = static_cast<std::uint64_t>(n / div);
    return low - quot * div;
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
    return cur;
#endif
}
template <class T>
LIBCPPRIME_CONSTEXPR T GCD(T x, T y) noexcept {
    if (x == 0) return 0;
    const std::int32_t n = CountrZero(x);
    const std::int32_t m = CountrZero(y);
    const std::int32_t l = n < m ? n : m;
    x >>= n;
    y >>= m;
    while (x != y) {
        const T a = y - x;
        const T b = x - y;
        const std::int32_t p = CountrZero(a);
        const std::int32_t q = CountrZero(b);
        Assume(p == q);
        const T s = y < x ? b : a;
        const T t = x < y ? x : y;
        x = s >> p;
        y = t;
    }
    return x << l;
}

template <bool Strict = false>
class MontgomeryModint64Impl {
    std::uint64_t mod_ = 0, rs = 0, nr = 0, np = 0;

    LIBCPPRIME_CONSTEXPR std::uint64_t reduce(const std::uint64_t n) const noexcept {
        std::uint64_t q = n * nr;
        if LIBCPPRIME_IF_CONSTEXPR (Strict) {
            std::uint64_t m = Mulu128High(q, mod_);
            return m == 0 ? 0 : mod_ - m;
        } else {
            std::uint64_t m = Mulu128High(q, mod_);
            return mod_ - m;
        }
    }
    LIBCPPRIME_CONSTEXPR std::uint64_t reduce(const std::uint64_t a, const std::uint64_t b) const noexcept {
        auto tmp = Mulu128(a, b);
        std::uint64_t d = tmp.high;
        std::uint64_t c = tmp.low;
        std::uint64_t q = c * nr;
        if LIBCPPRIME_IF_CONSTEXPR (Strict) {
            std::uint64_t m = Mulu128High(q, mod_);
            std::uint64_t t = d - m;
            return d < m ? t + mod_ : t;
        } else {
            std::uint64_t m = Mulu128High(q, mod_);
            return d + mod_ - m;
        }
    }

   public:
    LIBCPPRIME_CONSTEXPR MontgomeryModint64Impl(std::uint64_t n) noexcept {
        Assume(n > 2 && n % 2 != 0);
        mod_ = n;
        rs = Modu128(0xffffffffffffffff % n, 0 - n, n);
        nr = n;
        for (std::uint32_t i = 0; i != 5; ++i) nr *= 2 - n * nr;
        np = reduce(rs);
    }
    LIBCPPRIME_CONSTEXPR std::uint64_t build(std::uint32_t x) const noexcept { return reduce(x % mod_, rs); }
    LIBCPPRIME_CONSTEXPR std::uint64_t build(std::uint64_t x) const noexcept { return reduce(x % mod_, rs); }
    LIBCPPRIME_CONSTEXPR std::uint64_t raw(std::uint64_t x) const noexcept {
        Assume(x < mod_);
        return reduce(x, rs);
    }
    LIBCPPRIME_CONSTEXPR std::uint64_t val(std::uint64_t x) const noexcept {
        if LIBCPPRIME_IF_CONSTEXPR (Strict) {
            Assume(x < mod_);
            return reduce(x);
        } else {
            Assume(x < 2 * mod_);
            std::uint64_t tmp = reduce(x);
            return tmp - mod_ * (tmp >= mod_);
        }
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
    LIBCPPRIME_CONSTEXPR bool is_zero(std::uint64_t x) const noexcept {
        if LIBCPPRIME_IF_CONSTEXPR (Strict) {
            Assume(x < mod_);
            return x == 0;
        } else {
            Assume(x < 2 * mod_);
            return x == 0 || x == mod_;
        }
    }
    LIBCPPRIME_CONSTEXPR std::uint64_t add(std::uint64_t x, std::uint64_t y) const noexcept {
        if LIBCPPRIME_IF_CONSTEXPR (Strict) {
            Assume(x < mod_ && y < mod_);
            return x + y - (x >= mod_ - y) * mod_;
        } else {
            Assume(x < 2 * mod_ && y < 2 * mod_);
            return x + y - (x >= 2 * mod_ - y) * (2 * mod_);
        }
    }
    LIBCPPRIME_CONSTEXPR std::uint64_t sub(std::uint64_t x, std::uint64_t y) const noexcept {
        if LIBCPPRIME_IF_CONSTEXPR (Strict) {
            Assume(x < mod_ && y < mod_);
            return x - y + (x < y) * mod_;
        } else {
            Assume(x < 2 * mod_ && y < 2 * mod_);
            return x - y + (x < y) * (2 * mod_);
        }
    }
};

LIBCPPRIME_CONSTEXPR inline bool TrialDivision32(const std::uint32_t n) noexcept {
    return (n & 1) == 0 || 1431655766u > (0u - 1431655765u) * n || 858993460u > (0u - 858993459u) * n || 613566757u > (0u - 1227133513u) * n || 390451573u > (0u - 1171354717u) * n ||
           330382100u > (0u - 991146299u) * n || 252645136u > (0u - 252645135u) * n || 226050911u > 678152731u * n || 186737709u > (0u - 373475417u) * n;
}

constexpr std::uint16_t Bases32[256] = {
#include "./IsPrimeBases32.txt"
};
LIBCPPRIME_CONSTEXPR inline bool IsPrime32(const std::uint32_t x) noexcept {
    if (TrialDivision32(x)) return false;
    if (x < 85849) {
        const std::uint32_t a = static_cast<std::uint32_t>(Modu128(272518712866683587ull % x, 10755835586592736005ull, x));
        if (a == 0) return false;
        if (x < 11881) return GCD(a, x) == 1;
        const std::uint32_t b = static_cast<std::uint32_t>(Modu128(827936745744686818ull % x, 10132550402535125089ull, x));
        if (b == 0) return false;
        if (x < 39601) return GCD((a * b) % x, x) == 1;
        const std::uint32_t c = static_cast<std::uint32_t>(Modu128(9647383993136055606ull % x, 17068348107132031867ull, x) * a * b % x);
        if (c == 0) return false;
        return GCD(c, x) == 1;
    }
    const std::uint32_t h = x * 0xad625b89;
    std::uint32_t d = x - 1;
    std::uint32_t pw = static_cast<std::uint32_t>(Bases32[h >> 24]);
    std::int32_t s = CountrZero(d);
    d >>= s;
    if (x < (1u << 21)) {
        std::uint64_t m = 0xffffffffffffffff / x + 1;
        auto mul = [m, x](std::uint32_t a, std::uint32_t b) -> std::uint32_t { return static_cast<std::uint32_t>(Mulu128High(static_cast<std::uint64_t>(a) * b * m, x)); };
        std::uint32_t cur = pw;
        if (d != 1) {
            pw = mul(pw, pw);
            d >>= 1;
            while (d != 1) {
                std::uint32_t tmp = mul(pw, pw);
                if (d & 1) cur = mul(cur, pw);
                pw = tmp;
                d >>= 1;
            }
            cur = mul(cur, pw);
        }
        bool flag = cur == 1 || cur == x - 1;
        if (x % 4 == 3) return flag;
        if (flag) return true;
        while (--s) {
            cur = mul(cur, cur);
            if (cur == x - 1) return true;
        }
        return false;
    } else {
        std::uint32_t cur = pw;
        if (d != 1) {
            pw = static_cast<std::uint32_t>(std::uint64_t(pw) * pw % x);
            d >>= 1;
            while (d != 1) {
                std::uint32_t tmp = static_cast<std::uint32_t>(std::uint64_t(pw) * pw % x);
                if (d & 1) cur = static_cast<std::uint32_t>(std::uint64_t(cur) * pw % x);
                pw = tmp;
                d >>= 1;
            }
            cur = static_cast<std::uint32_t>(std::uint64_t(cur) * pw % x);
        }
        bool flag = cur == 1 || cur == x - 1;
        if (x % 4 == 3) return flag;
        if (flag) return true;
        while (--s) {
            cur = static_cast<std::uint32_t>(std::uint64_t(cur) * cur % x);
            if (cur == x - 1) return true;
        }
        return false;
    }
}

LIBCPPRIME_CONSTEXPR inline bool TrialDivision64(const std::uint64_t n) noexcept {
    return (n & 1) == 0 || 6148914691236517205u >= 12297829382473034411u * n || 3689348814741910323u >= 14757395258967641293u * n || 2635249153387078802u >= 7905747460161236407u * n ||
           1676976733973595601u >= 3353953467947191203u * n || 1418980313362273201u >= 5675921253449092805u * n || 1085102592571150095u >= 17361641481138401521u * n;
}

}  // namespace internal

}  // namespace cppr

#endif
