# Benchmark Summary

## Overall summary

- IsPrime averages 138.43 ns on prime inputs and 45.90 ns on composite inputs.
- IsPrimeNoTable averages 266.32 ns on prime inputs and 47.12 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 2.18 | 1.71 | 1.70 | 1.76 |
| 9-16 | 2.16 | 22.01 | 1.72 | 4.83 |
| 17-24 | 55.28 | 61.94 | 14.44 | 16.06 |
| 25-32 | 96.82 | 97.34 | 27.13 | 27.03 |
| 33-40 | 162.52 | 246.21 | 51.79 | 66.23 |
| 41-48 | 195.33 | 408.67 | 64.07 | 65.20 |
| 49-56 | 265.97 | 569.01 | 89.30 | 89.68 |
| 57-62 | 304.81 | 684.06 | 104.18 | 98.78 |
| 63-64 | 326.08 | 710.21 | 111.38 | 82.88 |
