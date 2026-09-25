# Benchmark Summary

## Overall summary

- IsPrime averages 155.47 ns on prime inputs and 51.18 ns on composite inputs.
- IsPrimeNoTable averages 301.82 ns on prime inputs and 53.51 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 2.25 | 2.02 | 2.00 | 2.00 |
| 9-16 | 2.25 | 23.07 | 2.01 | 5.17 |
| 17-24 | 61.71 | 68.99 | 15.52 | 17.78 |
| 25-32 | 109.13 | 109.47 | 29.97 | 30.31 |
| 33-40 | 181.52 | 280.12 | 57.31 | 74.67 |
| 41-48 | 218.16 | 473.83 | 70.97 | 75.98 |
| 49-56 | 299.81 | 647.41 | 100.22 | 102.97 |
| 57-62 | 344.55 | 765.13 | 117.20 | 110.86 |
| 63-64 | 365.50 | 793.42 | 124.97 | 92.67 |
