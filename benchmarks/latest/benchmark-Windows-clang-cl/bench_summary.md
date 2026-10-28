# Benchmark Summary

## Overall summary

- IsPrime averages 128.60 ns on prime inputs and 43.43 ns on composite inputs.
- IsPrimeCompact averages 252.82 ns on prime inputs and 46.25 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 4.01 | 2.35 | 4.15 | 2.50 |
| 9-16 | 4.19 | 19.37 | 4.19 | 4.43 |
| 17-24 | 35.73 | 66.18 | 11.05 | 16.53 |
| 25-32 | 119.74 | 124.02 | 33.44 | 33.47 |
| 33-40 | 161.15 | 239.59 | 51.64 | 63.72 |
| 41-48 | 193.71 | 407.27 | 63.78 | 64.95 |
| 49-56 | 226.64 | 520.59 | 76.93 | 78.56 |
| 57-62 | 254.81 | 586.06 | 87.98 | 89.84 |
| 63-64 | 307.84 | 689.26 | 105.68 | 110.01 |
