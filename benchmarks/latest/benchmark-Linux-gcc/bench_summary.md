# Benchmark Summary

## Overall summary

- IsPrime averages 142.72 ns on prime inputs and 47.28 ns on composite inputs.
- IsPrimeCompact averages 285.82 ns on prime inputs and 53.54 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 2.41 | 2.47 | 2.44 | 2.47 |
| 9-16 | 2.51 | 20.74 | 2.51 | 4.82 |
| 17-24 | 32.40 | 69.16 | 9.16 | 17.27 |
| 25-32 | 108.16 | 108.94 | 29.55 | 29.62 |
| 33-40 | 186.55 | 281.62 | 58.62 | 75.07 |
| 41-48 | 224.16 | 475.85 | 72.67 | 79.07 |
| 49-56 | 262.32 | 597.13 | 87.85 | 94.97 |
| 57-62 | 295.19 | 670.59 | 100.72 | 108.02 |
| 63-64 | 337.27 | 769.18 | 114.69 | 125.08 |
