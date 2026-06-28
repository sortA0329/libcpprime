# Benchmark Summary

## Overall summary

- IsPrime averages 137.54 ns on prime inputs and 45.58 ns on composite inputs.
- IsPrimeNoTable averages 250.91 ns on prime inputs and 42.91 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.91 | 3.18 | 1.90 | 3.17 |
| 9-16 | 1.90 | 21.49 | 1.90 | 5.22 |
| 17-24 | 55.98 | 63.96 | 14.18 | 16.62 |
| 25-32 | 96.64 | 97.40 | 26.54 | 27.11 |
| 33-40 | 153.02 | 236.53 | 48.86 | 64.45 |
| 41-48 | 183.39 | 394.27 | 60.18 | 65.89 |
| 49-56 | 267.04 | 517.31 | 89.62 | 72.99 |
| 57-62 | 308.00 | 607.94 | 105.33 | 73.69 |
| 63-64 | 369.93 | 744.92 | 126.08 | 90.47 |
