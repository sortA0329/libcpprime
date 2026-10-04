# Benchmark Summary

## Overall summary

- IsPrime averages 127.98 ns on prime inputs and 43.00 ns on composite inputs.
- IsPrimeCompact averages 252.64 ns on prime inputs and 47.10 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 3.04 | 1.85 | 3.02 | 1.80 |
| 9-16 | 3.03 | 19.61 | 3.04 | 3.96 |
| 17-24 | 30.03 | 61.15 | 9.02 | 15.00 |
| 25-32 | 97.95 | 96.64 | 27.17 | 25.97 |
| 33-40 | 166.73 | 249.17 | 53.09 | 66.30 |
| 41-48 | 200.25 | 420.48 | 65.61 | 69.66 |
| 49-56 | 234.11 | 528.53 | 79.22 | 83.80 |
| 57-62 | 263.54 | 593.43 | 90.67 | 95.35 |
| 63-64 | 301.77 | 669.09 | 103.29 | 109.85 |
