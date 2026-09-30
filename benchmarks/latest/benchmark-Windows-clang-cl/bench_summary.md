# Benchmark Summary

## Overall summary

- IsPrime averages 148.49 ns on prime inputs and 49.46 ns on composite inputs.
- IsPrimeCompact averages 256.06 ns on prime inputs and 47.71 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 3.20 | 2.04 | 3.27 | 2.18 |
| 9-16 | 3.25 | 20.42 | 3.20 | 4.30 |
| 17-24 | 57.55 | 62.26 | 14.91 | 15.52 |
| 25-32 | 99.37 | 99.21 | 27.60 | 26.66 |
| 33-40 | 170.34 | 252.09 | 54.23 | 67.24 |
| 41-48 | 204.80 | 423.84 | 67.05 | 70.33 |
| 49-56 | 290.66 | 533.51 | 97.32 | 84.44 |
| 57-62 | 333.67 | 599.10 | 114.13 | 96.09 |
| 63-64 | 361.30 | 696.06 | 123.88 | 110.41 |
