# Benchmark Summary

## Overall summary

- IsPrime averages 110.40 ns on prime inputs and 36.22 ns on composite inputs.
- IsPrimeCompact averages 221.14 ns on prime inputs and 40.22 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.72 | 1.35 | 1.45 | 1.36 |
| 9-16 | 1.70 | 17.69 | 1.40 | 3.83 |
| 17-24 | 25.37 | 53.73 | 6.76 | 13.68 |
| 25-32 | 84.94 | 84.93 | 22.92 | 23.37 |
| 33-40 | 145.24 | 215.22 | 45.29 | 57.31 |
| 41-48 | 174.67 | 367.28 | 56.27 | 58.87 |
| 49-56 | 204.21 | 472.13 | 68.15 | 71.44 |
| 57-62 | 229.80 | 532.40 | 78.11 | 81.70 |
| 63-64 | 237.59 | 520.01 | 80.93 | 83.62 |
