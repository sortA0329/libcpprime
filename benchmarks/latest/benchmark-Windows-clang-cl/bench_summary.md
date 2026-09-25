# Benchmark Summary

## Overall summary

- IsPrime averages 163.71 ns on prime inputs and 54.06 ns on composite inputs.
- IsPrimeNoTable averages 289.90 ns on prime inputs and 49.44 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 3.12 | 3.00 | 3.00 | 3.01 |
| 9-16 | 3.01 | 21.01 | 3.03 | 4.92 |
| 17-24 | 63.19 | 69.69 | 16.01 | 17.44 |
| 25-32 | 109.64 | 109.79 | 30.03 | 29.74 |
| 33-40 | 187.17 | 280.92 | 58.91 | 74.96 |
| 41-48 | 224.73 | 475.68 | 72.96 | 78.99 |
| 49-56 | 321.53 | 601.46 | 107.39 | 86.27 |
| 57-62 | 370.82 | 687.34 | 126.20 | 84.49 |
| 63-64 | 396.42 | 824.96 | 134.91 | 100.93 |
