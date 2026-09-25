# Benchmark Summary

## Overall summary

- IsPrime averages 147.12 ns on prime inputs and 48.98 ns on composite inputs.
- IsPrimeNoTable averages 258.25 ns on prime inputs and 44.77 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 3.09 | 2.94 | 3.03 | 2.98 |
| 9-16 | 3.08 | 21.11 | 3.08 | 5.25 |
| 17-24 | 56.77 | 62.48 | 14.79 | 16.45 |
| 25-32 | 97.66 | 98.75 | 27.25 | 27.57 |
| 33-40 | 169.49 | 249.92 | 53.84 | 67.72 |
| 41-48 | 203.65 | 421.77 | 66.62 | 70.93 |
| 49-56 | 287.83 | 535.06 | 96.41 | 77.38 |
| 57-62 | 330.94 | 612.47 | 113.12 | 75.86 |
| 63-64 | 356.62 | 730.86 | 121.83 | 90.22 |
