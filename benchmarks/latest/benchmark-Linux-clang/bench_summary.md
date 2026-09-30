# Benchmark Summary

## Overall summary

- IsPrime averages 156.21 ns on prime inputs and 51.40 ns on composite inputs.
- IsPrimeCompact averages 271.13 ns on prime inputs and 50.62 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 2.12 | 2.47 | 2.11 | 2.47 |
| 9-16 | 2.10 | 21.23 | 2.11 | 4.98 |
| 17-24 | 62.43 | 72.02 | 15.52 | 18.02 |
| 25-32 | 109.05 | 108.94 | 29.60 | 29.74 |
| 33-40 | 176.48 | 265.08 | 55.68 | 71.17 |
| 41-48 | 211.71 | 441.86 | 68.83 | 72.85 |
| 49-56 | 302.57 | 560.79 | 101.09 | 87.46 |
| 57-62 | 348.15 | 630.20 | 118.67 | 99.47 |
| 63-64 | 411.53 | 761.51 | 139.74 | 126.53 |
