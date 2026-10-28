# Benchmark Summary

## Overall summary

- IsPrime averages 126.43 ns on prime inputs and 42.26 ns on composite inputs.
- IsPrimeCompact averages 250.64 ns on prime inputs and 45.28 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 2.73 | 2.01 | 2.78 | 1.96 |
| 9-16 | 2.94 | 18.96 | 2.94 | 4.18 |
| 17-24 | 34.34 | 63.29 | 10.04 | 15.83 |
| 25-32 | 119.07 | 117.90 | 32.88 | 31.71 |
| 33-40 | 158.64 | 240.12 | 50.62 | 64.56 |
| 41-48 | 190.78 | 399.11 | 62.57 | 63.37 |
| 49-56 | 223.12 | 521.48 | 75.50 | 76.61 |
| 57-62 | 250.91 | 586.93 | 86.38 | 87.52 |
| 63-64 | 304.89 | 683.89 | 104.35 | 110.20 |
