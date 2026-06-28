# Benchmark Summary

## Overall summary

- IsPrime averages 137.53 ns on prime inputs and 45.57 ns on composite inputs.
- IsPrimeNoTable averages 250.81 ns on prime inputs and 42.68 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.90 | 2.87 | 1.93 | 2.90 |
| 9-16 | 1.90 | 21.36 | 1.91 | 4.97 |
| 17-24 | 56.05 | 63.96 | 14.19 | 16.42 |
| 25-32 | 96.68 | 97.39 | 26.54 | 26.92 |
| 33-40 | 153.03 | 236.46 | 48.85 | 64.20 |
| 41-48 | 183.40 | 394.15 | 60.17 | 65.66 |
| 49-56 | 266.97 | 517.17 | 89.61 | 72.78 |
| 57-62 | 307.86 | 607.88 | 105.29 | 73.48 |
| 63-64 | 369.92 | 744.82 | 126.06 | 90.27 |
