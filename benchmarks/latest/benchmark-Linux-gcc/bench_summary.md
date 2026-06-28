# Benchmark Summary

## Overall summary

- IsPrime averages 137.54 ns on prime inputs and 45.57 ns on composite inputs.
- IsPrimeNoTable averages 250.86 ns on prime inputs and 42.70 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.90 | 2.92 | 1.90 | 2.92 |
| 9-16 | 1.90 | 21.32 | 1.90 | 4.99 |
| 17-24 | 55.99 | 63.96 | 14.18 | 16.43 |
| 25-32 | 96.66 | 97.40 | 26.54 | 26.94 |
| 33-40 | 153.03 | 236.57 | 48.85 | 64.24 |
| 41-48 | 183.41 | 394.21 | 60.17 | 65.69 |
| 49-56 | 267.03 | 517.29 | 89.61 | 72.80 |
| 57-62 | 307.95 | 607.92 | 105.29 | 73.49 |
| 63-64 | 370.10 | 745.04 | 126.06 | 90.29 |
