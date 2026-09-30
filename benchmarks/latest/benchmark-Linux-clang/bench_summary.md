# Benchmark Summary

## Overall summary

- IsPrime averages 137.56 ns on prime inputs and 45.57 ns on composite inputs.
- IsPrimeCompact averages 241.08 ns on prime inputs and 45.39 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.91 | 2.29 | 1.90 | 2.29 |
| 9-16 | 1.90 | 21.14 | 1.90 | 4.74 |
| 17-24 | 56.08 | 66.65 | 14.18 | 16.83 |
| 25-32 | 96.63 | 97.12 | 26.53 | 26.66 |
| 33-40 | 153.01 | 235.10 | 48.86 | 63.34 |
| 41-48 | 183.40 | 392.98 | 60.17 | 65.42 |
| 49-56 | 267.13 | 497.25 | 89.62 | 78.46 |
| 57-62 | 307.98 | 558.26 | 105.30 | 89.16 |
| 63-64 | 369.91 | 670.16 | 126.07 | 111.08 |
