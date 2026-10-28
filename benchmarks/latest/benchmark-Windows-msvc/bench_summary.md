# Benchmark Summary

## Overall summary

- IsPrime averages 107.63 ns on prime inputs and 35.96 ns on composite inputs.
- IsPrimeCompact averages 221.06 ns on prime inputs and 40.28 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 2.10 | 1.42 | 2.17 | 1.38 |
| 9-16 | 2.19 | 17.67 | 2.18 | 3.93 |
| 17-24 | 25.71 | 53.69 | 7.45 | 13.78 |
| 25-32 | 84.72 | 84.78 | 23.46 | 23.43 |
| 33-40 | 140.77 | 215.45 | 44.66 | 57.35 |
| 41-48 | 168.91 | 367.20 | 55.19 | 58.93 |
| 49-56 | 197.36 | 472.01 | 66.61 | 71.50 |
| 57-62 | 222.05 | 532.74 | 76.16 | 81.76 |
| 63-64 | 238.10 | 516.81 | 81.67 | 83.66 |
