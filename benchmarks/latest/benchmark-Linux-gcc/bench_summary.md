# Benchmark Summary

## Overall summary

- IsPrime averages 120.31 ns on prime inputs and 40.04 ns on composite inputs.
- IsPrimeCompact averages 239.89 ns on prime inputs and 45.06 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.90 | 2.25 | 1.90 | 2.26 |
| 9-16 | 1.90 | 21.08 | 1.90 | 4.79 |
| 17-24 | 28.93 | 66.69 | 8.09 | 16.91 |
| 25-32 | 96.37 | 97.16 | 26.47 | 26.73 |
| 33-40 | 155.79 | 234.12 | 49.46 | 63.21 |
| 41-48 | 186.79 | 389.00 | 61.07 | 64.53 |
| 49-56 | 218.17 | 494.38 | 73.57 | 77.38 |
| 57-62 | 245.19 | 555.00 | 84.11 | 87.92 |
| 63-64 | 295.86 | 673.80 | 100.98 | 112.32 |
