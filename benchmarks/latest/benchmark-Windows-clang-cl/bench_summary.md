# Benchmark Summary

## Overall summary

- IsPrime averages 147.27 ns on prime inputs and 48.90 ns on composite inputs.
- IsPrimeNoTable averages 258.03 ns on prime inputs and 44.45 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 3.03 | 2.82 | 3.05 | 2.83 |
| 9-16 | 3.08 | 21.08 | 3.07 | 4.97 |
| 17-24 | 56.74 | 62.54 | 14.70 | 16.13 |
| 25-32 | 98.14 | 98.81 | 27.12 | 27.32 |
| 33-40 | 169.72 | 249.61 | 53.78 | 67.41 |
| 41-48 | 203.94 | 421.55 | 66.57 | 70.59 |
| 49-56 | 288.02 | 535.08 | 96.32 | 76.95 |
| 57-62 | 331.10 | 611.95 | 112.96 | 75.33 |
| 63-64 | 356.41 | 727.38 | 121.63 | 89.90 |
