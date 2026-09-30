# Benchmark Summary

## Overall summary

- IsPrime averages 137.54 ns on prime inputs and 45.57 ns on composite inputs.
- IsPrimeCompact averages 241.09 ns on prime inputs and 45.49 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.90 | 2.41 | 1.90 | 2.40 |
| 9-16 | 1.90 | 21.20 | 1.90 | 4.86 |
| 17-24 | 55.98 | 66.65 | 14.17 | 16.93 |
| 25-32 | 96.68 | 97.12 | 26.54 | 26.76 |
| 33-40 | 153.01 | 235.11 | 48.86 | 63.42 |
| 41-48 | 183.41 | 392.92 | 60.17 | 65.51 |
| 49-56 | 267.05 | 497.28 | 89.62 | 78.54 |
| 57-62 | 307.98 | 558.16 | 105.31 | 89.24 |
| 63-64 | 369.94 | 670.38 | 126.08 | 111.16 |
