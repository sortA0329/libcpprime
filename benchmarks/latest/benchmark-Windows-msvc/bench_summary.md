# Benchmark Summary

## Overall summary

- IsPrime averages 104.68 ns on prime inputs and 33.59 ns on composite inputs.
- IsPrimeCompact averages 176.38 ns on prime inputs and 32.32 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.02 | 1.02 | 1.00 | 1.01 |
| 9-16 | 1.01 | 12.40 | 1.01 | 2.37 |
| 17-24 | 45.09 | 47.08 | 10.69 | 11.14 |
| 25-32 | 107.17 | 107.79 | 28.06 | 27.88 |
| 33-40 | 114.87 | 163.56 | 35.74 | 42.53 |
| 41-48 | 139.06 | 285.96 | 44.64 | 45.58 |
| 49-56 | 192.01 | 359.18 | 63.64 | 55.42 |
| 57-62 | 220.54 | 406.59 | 74.54 | 63.61 |
| 63-64 | 235.37 | 428.67 | 79.55 | 68.49 |
