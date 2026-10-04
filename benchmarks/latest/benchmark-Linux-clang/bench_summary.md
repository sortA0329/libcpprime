# Benchmark Summary

## Overall summary

- IsPrime averages 120.30 ns on prime inputs and 40.05 ns on composite inputs.
- IsPrimeCompact averages 239.88 ns on prime inputs and 45.09 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.93 | 2.28 | 1.90 | 2.27 |
| 9-16 | 1.91 | 21.08 | 1.91 | 4.84 |
| 17-24 | 28.94 | 66.74 | 8.10 | 16.96 |
| 25-32 | 96.33 | 97.08 | 26.45 | 26.76 |
| 33-40 | 155.83 | 234.13 | 49.46 | 63.23 |
| 41-48 | 186.75 | 389.06 | 61.08 | 64.55 |
| 49-56 | 218.16 | 494.37 | 73.58 | 77.41 |
| 57-62 | 245.09 | 554.93 | 84.12 | 87.94 |
| 63-64 | 295.78 | 673.70 | 100.99 | 112.33 |
