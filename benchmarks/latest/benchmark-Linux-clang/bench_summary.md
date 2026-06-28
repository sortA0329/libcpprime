# Benchmark Summary

## Overall summary

- IsPrime averages 137.54 ns on prime inputs and 45.58 ns on composite inputs.
- IsPrimeNoTable averages 250.86 ns on prime inputs and 42.88 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.91 | 3.15 | 1.90 | 3.22 |
| 9-16 | 1.90 | 21.40 | 1.91 | 5.18 |
| 17-24 | 55.99 | 63.95 | 14.19 | 16.58 |
| 25-32 | 96.63 | 97.37 | 26.56 | 27.07 |
| 33-40 | 153.03 | 236.53 | 48.85 | 64.40 |
| 41-48 | 183.39 | 394.15 | 60.17 | 65.85 |
| 49-56 | 267.04 | 517.21 | 89.61 | 72.96 |
| 57-62 | 307.91 | 607.90 | 105.30 | 73.65 |
| 63-64 | 370.11 | 744.85 | 126.06 | 90.44 |
