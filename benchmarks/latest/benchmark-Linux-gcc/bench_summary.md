# Benchmark Summary

## Overall summary

- IsPrime averages 137.22 ns on prime inputs and 45.37 ns on composite inputs.
- IsPrimeNoTable averages 248.65 ns on prime inputs and 42.45 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.90 | 2.92 | 1.90 | 2.93 |
| 9-16 | 1.90 | 21.30 | 1.90 | 5.11 |
| 17-24 | 55.64 | 63.90 | 14.19 | 16.55 |
| 25-32 | 96.66 | 97.40 | 26.65 | 27.04 |
| 33-40 | 153.37 | 235.30 | 48.67 | 63.88 |
| 41-48 | 183.72 | 390.31 | 59.99 | 64.84 |
| 49-56 | 266.08 | 511.83 | 89.13 | 72.05 |
| 57-62 | 307.02 | 600.41 | 104.77 | 72.99 |
| 63-64 | 365.10 | 740.87 | 124.18 | 90.28 |
