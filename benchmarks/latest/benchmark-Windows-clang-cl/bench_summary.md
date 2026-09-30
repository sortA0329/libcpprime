# Benchmark Summary

## Overall summary

- IsPrime averages 144.46 ns on prime inputs and 47.30 ns on composite inputs.
- IsPrimeCompact averages 234.19 ns on prime inputs and 44.65 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeCompact (prime) | IsPrime (composite) | IsPrimeCompact (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 2.41 | 1.43 | 2.39 | 1.34 |
| 9-16 | 2.35 | 17.68 | 2.36 | 3.40 |
| 17-24 | 60.08 | 64.22 | 15.06 | 15.37 |
| 25-32 | 137.99 | 139.89 | 36.93 | 36.45 |
| 33-40 | 150.22 | 223.19 | 47.53 | 59.63 |
| 41-48 | 179.50 | 380.37 | 58.62 | 63.78 |
| 49-56 | 276.93 | 469.47 | 92.49 | 76.42 |
| 57-62 | 321.98 | 525.58 | 109.34 | 86.78 |
| 63-64 | 348.03 | 615.99 | 119.21 | 99.67 |
