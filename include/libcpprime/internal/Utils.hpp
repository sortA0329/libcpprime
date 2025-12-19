/**
 *
 * libcpprime https://github.com/Rac75116/libcpprime
 *
 * Copyright (c) 2025 Rac75116
 * SPDX-License-Identifier: MIT
 *
 **/
// This file contains code derived from libdivide
// https://libdivide.com
//
// Original work:
// Copyright (C) 2010 - 2022 ridiculous_fish <libdivide@ridiculousfish.com>
// Copyright (C) 2016 - 2022 Kim Walisch <kim.walisch@gmail.com>
//
// libdivide is dual-licensed under the Boost Software License 1.0
// and the zlib License. This project uses the Boost Software License 1.0.
//
// The original code has been modified and integrated into this library.
// Modifications include refactoring and API replacement.
//
// Except for the portions derived from libdivide, the remainder of this
// library is licensed under the MIT License.

#ifndef LIBCPPRIME_INCLUDED_INTERNAL_UTILS
#define LIBCPPRIME_INCLUDED_INTERNAL_UTILS

#include <bit>
#include <cstdint>
#include <type_traits>

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC))
#if !defined(__clang__) || _MSC_VER > 1930
#include <intrin.h>
#define LIBCPPRIME_VC_MUL_INTRINSICS
#pragma intrinsic(_umul128)
#pragma intrinsic(__umulh)
#endif
#if !defined(__clang__) && _MSC_VER >= 1920
#include <immintrin.h>
#define LIBCPPRIME_VC_DIV_INTRINSICS
#pragma intrinsic(_udiv128)
#endif
#endif

#ifdef __has_builtin
#define LIBCPPRIME_HAS_BUILTIN(x) __has_builtin(x)
#else
#define LIBCPPRIME_HAS_BUILTIN(x) 0
#endif

#ifdef __SIZEOF_INT128__
#define LIBCPPRIME_HAS_INT128_T
#if !(defined(__clang__) && defined(_MSC_VER))
#define LIBCPPRIME_HAS_INT128_RUNTIME_DIV
#endif
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define LIBCPPRIME_X86_64
#endif

#if defined(__GNUC__) || defined(__clang__)
#define LIBCPPRIME_GCC_STYLE_ASM
#endif

#ifdef __has_attribute
#if __has_attribute(always_inline)
#define LIBCPPRIME_INLINE __attribute__((always_inline)) inline
#endif
#endif
#ifndef LIBCPPRIME_INLINE
#ifdef _MSC_VER
#define LIBCPPRIME_INLINE __forceinline
#else
#define LIBCPPRIME_INLINE inline
#endif
#endif

namespace cppr {

namespace internal {

#if defined(LIBCPPRIME_HAS_INT128_T)
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

constexpr LIBCPPRIME_INLINE void Assume([[maybe_unused]] const bool f) noexcept {
    // Hint for the optimizer; ignored during constant evaluation.
    if (std::is_constant_evaluated()) return;
#if LIBCPPRIME_HAS_BUILTIN(__builtin_assume)
    __builtin_assume(f);
#elif LIBCPPRIME_HAS_BUILTIN(__builtin_unreachable)
    if (!f) __builtin_unreachable();
#elif defined(_MSC_VER)
    __assume(f);
#endif
}

constexpr LIBCPPRIME_INLINE std::int32_t CountrZero(std::uint64_t n) noexcept {
    // Precondition: n != 0 (matches std::countr_zero requirements).
    Assume(n != 0);
    return std::countr_zero(n);
}
constexpr LIBCPPRIME_INLINE std::int32_t CountlZero(std::uint64_t n) noexcept {
    // Precondition: n != 0 (matches std::countl_zero requirements).
    Assume(n != 0);
    return std::countl_zero(n);
}

constexpr LIBCPPRIME_INLINE Int64Pair Mulu128(std::uint64_t muler, std::uint64_t mulnd) noexcept {
    // Full 128-bit product split into {high, low}.
#if defined(LIBCPPRIME_HAS_INT128_T)
    UInt128 tmp = static_cast<UInt128>(muler) * mulnd;
    return {static_cast<std::uint64_t>(tmp >> 64), static_cast<std::uint64_t>(tmp)};
#else
#if defined(LIBCPPRIME_VC_MUL_INTRINSICS)
    if (!std::is_constant_evaluated()) {
        std::uint64_t high;
        std::uint64_t low = _umul128(muler, mulnd, &high);
        return {high, low};
    }
#endif
    std::uint32_t lhigh = muler >> 32;
    std::uint32_t llow = muler & 0xffffffff;
    std::uint32_t rhigh = mulnd >> 32;
    std::uint32_t rlow = mulnd & 0xffffffff;
    std::uint64_t mlh = static_cast<std::uint64_t>(llow) * rhigh;
    std::uint64_t mhl = static_cast<std::uint64_t>(lhigh) * rlow;
    std::uint64_t ma = mlh + mhl;
    std::uint64_t mll = static_cast<std::uint64_t>(llow) * rlow;
    std::uint64_t mhh = static_cast<std::uint64_t>(lhigh) * rhigh;
    std::uint64_t carry1 = static_cast<std::uint64_t>(ma < mlh) << 32;
    std::uint64_t rl = mll + (ma << 32);
    std::uint64_t carry2 = rl < mll;
    return {mhh + carry1 + carry2 + (ma >> 32), rl};
#endif
}

constexpr LIBCPPRIME_INLINE std::uint64_t Mulu128High(std::uint64_t muler, std::uint64_t mulnd) noexcept {
    // High 64 bits of the 128-bit product.
#if defined(LIBCPPRIME_HAS_INT128_T)
    return static_cast<std::uint64_t>((static_cast<UInt128>(muler) * mulnd) >> 64);
#else
#if defined(LIBCPPRIME_VC_MUL_INTRINSICS)
    if (!std::is_constant_evaluated()) return __umulh(muler, mulnd);
#endif
    return Mulu128(muler, mulnd).high;
#endif
}

constexpr std::uint64_t Modu128(std::uint64_t numhi, std::uint64_t numlo, std::uint64_t den) {
    // Computes ((numhi << 64) | numlo) % den.
    // Preconditions: den != 0 and numhi < den (so the quotient fits in 64 bits).
    Assume(den != 0);
    Assume(numhi < den);

#if defined(LIBCPPRIME_X86_64) && defined(LIBCPPRIME_GCC_STYLE_ASM)
    if (!std::is_constant_evaluated()) {
        std::uint64_t quot, rem;
        __asm__("div %[v]" : "=a"(quot), "=d"(rem) : [v] "r"(den), "a"(numlo), "d"(numhi));
        return rem;
    }
#elif defined(LIBCPPRIME_VC_DIV_INTRINSICS)
    if (!std::is_constant_evaluated()) {
        std::uint64_t rem = 0;
        _udiv128(numhi, numlo, den, &rem);
        return rem;
    }
#endif
#if defined(LIBCPPRIME_HAS_INT128_RUNTIME_DIV)
    UInt128 tmp = (static_cast<UInt128>(numhi) << 64) | numlo;
    return static_cast<std::uint64_t>(tmp % den);
#else
#if defined(LIBCPPRIME_HAS_INT128_T)
    if (std::is_constant_evaluated()) {
        UInt128 tmp = (static_cast<UInt128>(numhi) << 64) | numlo;
        return static_cast<std::uint64_t>(tmp % den);
    }
#endif
    constexpr std::uint64_t b = (static_cast<std::uint64_t>(1) << 32);
    std::uint32_t q1, q0;
    std::int32_t shift;
    std::uint32_t den1, den0, num1, num0;
    std::uint64_t rem, qhat, rhat, c1, c2;
    shift = CountlZero(den);
    den <<= shift;
    numhi <<= shift;
    numhi |= (numlo >> (-shift & 63)) & static_cast<std::uint64_t>(-static_cast<std::int64_t>(shift) >> 63);
    numlo <<= shift;
    num1 = static_cast<std::uint32_t>(numlo >> 32);
    num0 = static_cast<std::uint32_t>(numlo & 0xFFFFFFFFu);
    den1 = static_cast<std::uint32_t>(den >> 32);
    den0 = static_cast<std::uint32_t>(den & 0xFFFFFFFFu);
    qhat = numhi / den1;
    rhat = numhi % den1;
    c1 = qhat * den0;
    c2 = rhat * b + num1;
    if (c1 > c2) qhat -= (c1 - c2 > den) ? 2 : 1;
    q1 = static_cast<std::uint32_t>(qhat);
    rem = numhi * b + num1 - q1 * den;
    qhat = rem / den1;
    rhat = rem % den1;
    c1 = qhat * den0;
    c2 = rhat * b + num0;
    if (c1 > c2) qhat -= (c1 - c2 > den) ? 2 : 1;
    q0 = static_cast<std::uint32_t>(qhat);
    return (rem * b + num0 - q0 * den) >> shift;
#endif
}

template <class T>
constexpr T GCD(T x, T y) noexcept {
    // Binary GCD (Stein's algorithm). Assumes y != 0 when x != 0.
    if (x == 0) return 0;
    Assume(y != 0);
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

}  // namespace internal

}  // namespace cppr

#endif
