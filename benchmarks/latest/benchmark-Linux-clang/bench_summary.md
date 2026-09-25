# Benchmark Summary

## Overall summary

- IsPrime averages 137.20 ns on prime inputs and 45.36 ns on composite inputs.
- IsPrimeNoTable averages 248.66 ns on prime inputs and 42.52 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.90 | 2.99 | 1.90 | 3.00 |
| 9-16 | 1.90 | 21.36 | 1.90 | 5.14 |
| 17-24 | 55.65 | 63.89 | 14.17 | 16.57 |
| 25-32 | 96.63 | 97.39 | 26.66 | 27.05 |
| 33-40 | 153.38 | 235.30 | 48.67 | 63.98 |
| 41-48 | 183.74 | 390.31 | 59.99 | 64.94 |
| 49-56 | 266.02 | 511.84 | 89.10 | 72.15 |
| 57-62 | 306.88 | 600.38 | 104.76 | 73.09 |
| 63-64 | 365.11 | 740.88 | 124.17 | 90.39 |
