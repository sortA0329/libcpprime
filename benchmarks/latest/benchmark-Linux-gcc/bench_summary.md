# Benchmark Summary

## Overall summary

- IsPrime averages 137.53 ns on prime inputs and 45.58 ns on composite inputs.
- IsPrimeCompact averages 241.07 ns on prime inputs and 45.36 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.90 | 2.28 | 1.91 | 2.25 |
| 9-16 | 1.90 | 21.13 | 1.90 | 4.70 |
| 17-24 | 55.99 | 66.60 | 14.19 | 16.79 |
| 25-32 | 96.63 | 97.16 | 26.55 | 26.63 |
| 33-40 | 153.04 | 235.15 | 48.85 | 63.31 |
| 41-48 | 183.40 | 392.95 | 60.18 | 65.39 |
| 49-56 | 267.03 | 497.23 | 89.63 | 78.42 |
| 57-62 | 307.92 | 558.19 | 105.31 | 89.13 |
| 63-64 | 369.90 | 670.16 | 126.08 | 111.05 |
