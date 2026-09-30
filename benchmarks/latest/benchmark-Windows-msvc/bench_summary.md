# Benchmark Summary

## Overall summary

- IsPrime averages 155.79 ns on prime inputs and 51.21 ns on composite inputs.
- IsPrimeCompact averages 284.72 ns on prime inputs and 50.92 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 2.27 | 2.01 | 2.02 | 2.00 |
| 9-16 | 2.22 | 22.85 | 2.01 | 4.27 |
| 17-24 | 61.75 | 69.23 | 15.67 | 16.72 |
| 25-32 | 109.33 | 109.24 | 30.20 | 29.18 |
| 33-40 | 181.69 | 277.85 | 57.29 | 72.68 |
| 41-48 | 218.38 | 472.57 | 70.94 | 74.69 |
| 49-56 | 300.30 | 607.81 | 100.20 | 90.87 |
| 57-62 | 345.60 | 685.06 | 117.22 | 104.08 |
| 63-64 | 367.88 | 668.37 | 124.68 | 106.59 |
