# Benchmark Summary

## Overall summary

- IsPrime averages 120.85 ns on prime inputs and 39.73 ns on composite inputs.
- IsPrimeNoTable averages 234.53 ns on prime inputs and 41.59 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.68 | 1.37 | 1.36 | 1.40 |
| 9-16 | 1.68 | 17.86 | 1.40 | 4.02 |
| 17-24 | 47.92 | 53.53 | 11.98 | 13.86 |
| 25-32 | 84.84 | 85.17 | 23.28 | 23.58 |
| 33-40 | 141.05 | 217.53 | 44.53 | 58.07 |
| 41-48 | 169.53 | 368.15 | 55.16 | 59.05 |
| 49-56 | 233.05 | 502.96 | 77.90 | 80.04 |
| 57-62 | 267.94 | 595.36 | 91.19 | 86.19 |
| 63-64 | 284.80 | 616.11 | 96.98 | 72.02 |
