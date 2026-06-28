# Benchmark Summary

## Overall summary

- IsPrime averages 147.01 ns on prime inputs and 48.83 ns on composite inputs.
- IsPrimeNoTable averages 257.42 ns on prime inputs and 44.25 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 3.07 | 2.72 | 3.02 | 2.71 |
| 9-16 | 3.05 | 20.99 | 3.03 | 4.88 |
| 17-24 | 56.72 | 62.11 | 14.65 | 16.03 |
| 25-32 | 97.70 | 98.23 | 27.04 | 27.07 |
| 33-40 | 169.51 | 249.03 | 53.72 | 67.14 |
| 41-48 | 203.60 | 420.61 | 66.50 | 70.37 |
| 49-56 | 287.36 | 534.07 | 96.23 | 76.71 |
| 57-62 | 330.70 | 611.11 | 112.87 | 75.09 |
| 63-64 | 356.30 | 725.69 | 121.49 | 89.70 |
