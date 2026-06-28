# Benchmark Summary

## Overall summary

- IsPrime averages 120.71 ns on prime inputs and 39.82 ns on composite inputs.
- IsPrimeNoTable averages 239.78 ns on prime inputs and 43.29 ns on composite inputs.

## Averages by 8-bit range (nanoseconds)

| Bit range | IsPrime (prime) | IsPrimeNoTable (prime) | IsPrime (composite) | IsPrimeNoTable (composite) |
|-----------|-----------------|------------------------|---------------------|----------------------------|
| 1-8 | 1.69 | 1.67 | 1.41 | 1.67 |
| 9-16 | 1.67 | 18.07 | 1.38 | 3.89 |
| 17-24 | 47.87 | 53.99 | 12.20 | 13.68 |
| 25-32 | 84.71 | 84.88 | 23.47 | 23.36 |
| 33-40 | 140.94 | 217.56 | 44.69 | 57.84 |
| 41-48 | 169.45 | 367.85 | 55.30 | 58.87 |
| 49-56 | 232.79 | 502.55 | 77.94 | 79.73 |
| 57-62 | 267.56 | 648.05 | 91.14 | 105.15 |
| 63-64 | 283.91 | 623.49 | 96.89 | 72.06 |
