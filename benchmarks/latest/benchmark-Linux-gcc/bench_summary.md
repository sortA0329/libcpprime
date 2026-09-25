# Benchmark Summary

## Overall summary

- IsPrime averages 137.21 ns on prime inputs and 45.45 ns on composite inputs.
- IsPrimeNoTable averages 248.89 ns on prime inputs and 43.83 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.90 | 4.10 | 1.92 | 4.14 |
| 9-16 | 1.91 | 21.94 | 1.91 | 6.69 |
| 17-24 | 55.64 | 63.94 | 14.26 | 17.97 |
| 25-32 | 96.62 | 97.52 | 26.74 | 28.42 |
| 33-40 | 153.37 | 235.31 | 48.80 | 65.27 |
| 41-48 | 183.69 | 390.32 | 60.12 | 66.22 |
| 49-56 | 266.14 | 511.82 | 89.24 | 73.40 |
| 57-62 | 306.91 | 600.43 | 104.90 | 74.32 |
| 63-64 | 365.13 | 740.87 | 124.29 | 91.61 |
