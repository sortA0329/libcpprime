# Benchmark Summary

## Overall summary

- IsPrime averages 137.22 ns on prime inputs and 45.36 ns on composite inputs.
- IsPrimeNoTable averages 248.75 ns on prime inputs and 42.87 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.90 | 3.23 | 1.91 | 3.23 |
| 9-16 | 1.90 | 21.37 | 1.90 | 5.54 |
| 17-24 | 55.68 | 63.89 | 14.18 | 16.95 |
| 25-32 | 96.67 | 97.39 | 26.64 | 27.40 |
| 33-40 | 153.44 | 235.30 | 48.67 | 64.34 |
| 41-48 | 183.70 | 390.41 | 59.99 | 65.30 |
| 49-56 | 266.11 | 511.81 | 89.12 | 72.50 |
| 57-62 | 306.87 | 600.43 | 104.76 | 73.44 |
| 63-64 | 365.16 | 742.39 | 124.15 | 90.72 |
